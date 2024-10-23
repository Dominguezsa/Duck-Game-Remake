#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/Chunk.hh>
#include <SDL2pp/SDL2pp.hh>
#include <SDL2pp/Font.hh>
#include <SDL2pp/Renderer.hh>
#include <SDL2pp/Texture.hh>
#include <SDL2pp/Window.hh>
#include <SDL2pp/Mixer.hh>
#include <SDL2pp/Music.hh>

using namespace SDL2pp;

// Structure to represent a platform
struct Platform {
    float x;
    float y;
    float width;
    float height;
};

// Function to check if the duck collides with a platform
bool checkPlatformCollision(float duck_x, float duck_y, float duck_width, float duck_height, const Platform& platform) {
    // Define duck's collision box (slightly smaller than sprite for better gameplay)
    float duck_collision_x = duck_x + duck_width/4;
    float duck_collision_width = duck_width/2;
    
    // Check for full collision box overlap
    bool vertical_overlap = (duck_y + duck_height >= platform.y) && (duck_y <= platform.y + platform.height);
    bool horizontal_overlap = (duck_collision_x + duck_collision_width >= platform.x) && 
                            (duck_collision_x <= platform.x + platform.width);
    
    return vertical_overlap && horizontal_overlap;
}

int main() try {
    // Initialize SDL library
    SDL sdl(SDL_INIT_VIDEO);

    // Initialize SDL_ttf library
    SDLTTF ttf;

    // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
    Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1200, 700,
                  SDL_WINDOW_RESIZABLE);
    
    // El mixer para testear el audio
    Mixer mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 409600);
    // Load music file
    Music music("./data/audio/ost/back_music_space_mystery_out.ogg");
    // Loading SFX files
    Chunk boom1("./data/audio/sfx/boom1.wav");
    Chunk boom2("./data/audio/sfx/boom2.wav");
    Chunk boom3("./data/audio/sfx/boom3.wav");

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load sprites and textures
    Texture sprites(renderer, Surface("data/imagenesDePatos.png").SetColorKey(true, 0));
    sprites.SetBlendMode(SDL_BLENDMODE_BLEND);
    Texture background(renderer, Surface("data/fondo.png"));
    Texture tablas(renderer, Surface("data/tablon1.png"));
    Font font("data/Vera.ttf", 12);

    // Game state variables
    bool is_running = false;
    int run_phase = -1;
    float position = 0.0;
    bool looking_right = true;
    bool shift = false;
    bool agachado = false;
    bool is_jumping = false;
    bool space_pressed = false;
    bool space_released = true;

    // Physics variables
    float vertical_velocity = 0.0f;
    const float gravity = 0.5f;
    const float jump_force = -10.0f;
    const float flutter_force = -0.3f;
    const float max_fall_speed = 15.0f;
    float y_position = renderer.GetOutputHeight() / 2 - 64;

    // Calculate platform dimensions
    int cell_width = renderer.GetOutputWidth() / 12;
    int cell_height = renderer.GetOutputHeight() / 14;
    
    // Define platforms
    Platform left_platform = {
        0,                                    
        (float)renderer.GetOutputHeight() / 2,  
        (float)renderer.GetOutputWidth() / 2,   
        (float)cell_height                      
    };
    
    Platform right_platform = {
        (float)renderer.GetOutputWidth() / 2,                 
        (float)renderer.GetOutputHeight() / 2 + cell_height * 3, 
        (float)renderer.GetOutputWidth() / 2,                 
        (float)cell_height                                    
    };

    unsigned int prev_ticks = SDL_GetTicks();

    // Audio setup
    mixer.SetMusicVolume(30);
    mixer.SetVolume(-1, 20);
    mixer.PlayMusic(music, -1);

    // Main loop
    while (1) {
        unsigned int frame_ticks = SDL_GetTicks();
        unsigned int frame_delta = SDL_GetTicks() - prev_ticks;
        prev_ticks = frame_ticks;

        // Event processing
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                return 0;
            } else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                    case SDLK_q:
                        return 0;
                    case SDLK_RIGHT:
                        is_running = true;
                        looking_right = true;
                        break;
                    case SDLK_LEFT:
                        is_running = true;
                        looking_right = false;
                        break;
                    case SDLK_DOWN:
                        agachado = true;
                        break;
                    case SDLK_SPACE:
                        if (!space_pressed && space_released) {
                            space_pressed = true;
                            space_released = false;
                            if (!is_jumping) {
                                is_jumping = true;
                                vertical_velocity = jump_force;
                            }
                        }
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shift = true;
                        break;
                    case SDLK_1:
                        mixer.PlayChannel(-1, boom1, 1);
                        break;
                    case SDLK_2:
                        mixer.PlayChannel(-1, boom2, 0);
                        break;
                    case SDLK_3:
                        mixer.PlayChannel(-1, boom3, 0);
                        break;
                }
            } else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_RIGHT:
                        is_running = false;
                        break;
                    case SDLK_LEFT:
                        is_running = false;
                        break;
                    case SDLK_DOWN:
                        agachado = false;
                        break;
                    case SDLK_SPACE:
                        space_pressed = false;
                        space_released = true;
                        break;
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shift = false;
                        break;
                }
            }
        }

        float old_position = position;

        // Update horizontal position
        if (is_running && !agachado) {
            float move_speed = frame_delta * 0.2f * (shift ? 3 : 1);
            if (looking_right) {
                position += move_speed;
            } else {
                position -= move_speed;
            }
            run_phase = (frame_ticks / 100) % 5;
        } else if (!agachado) {
            run_phase = 0;
        }

        // Wrap player position if out of bounds
        if (position > renderer.GetOutputWidth())
            position = -50;
        if (position < -50)
            position = renderer.GetOutputWidth();

        // Apply gravity and handle platform collision
        vertical_velocity += gravity;
        
        // Clamp maximum fall speed
        if (vertical_velocity > max_fall_speed) {
            vertical_velocity = max_fall_speed;
        }
        
        // Apply flutter effect if space is held during jump
        if (space_pressed && is_jumping) {
            vertical_velocity += flutter_force;
            // Clamp maximum fall speed during flutter
            if (vertical_velocity > 3.0f) {
                vertical_velocity = 3.0f;
            }
        }

        // Store previous position for collision resolution
        float previous_y = y_position;
        
        // Update vertical position
        y_position += vertical_velocity;

        // Check collision with platforms
        bool on_platform = false;
        
        // Check collision with left platform
        if (checkPlatformCollision(position, y_position, 64, 64, left_platform)) {
            if (vertical_velocity > 0 && previous_y + 64 <= left_platform.y) {
                // Landing on top of platform
                y_position = left_platform.y - 64;
                vertical_velocity = 0;
                is_jumping = false;
                on_platform = true;
            } else if (vertical_velocity < 0 && previous_y >= left_platform.y + left_platform.height) {
                // Hitting platform from below
                y_position = left_platform.y + left_platform.height;
                vertical_velocity = 0;
            } else {
                // Side collision
                position = old_position;
            }
        }
        
        // Check collision with right platform
        if (checkPlatformCollision(position, y_position, 64, 64, right_platform)) {
            if (vertical_velocity > 0 && previous_y + 64 <= right_platform.y) {
                // Landing on top of platform
                y_position = right_platform.y - 64;
                vertical_velocity = 0;
                is_jumping = false;
                on_platform = true;
            } else if (vertical_velocity < 0 && previous_y >= right_platform.y + right_platform.height) {
                // Hitting platform from below
                y_position = right_platform.y + right_platform.height;
                vertical_velocity = 0;
            } else {
                // Side collision
                position = old_position;
            }
        }

        // Bottom screen boundary
        float ground_level = renderer.GetOutputHeight() - 64;
        if (y_position > ground_level) {
            y_position = ground_level;
            vertical_velocity = 0;
            is_jumping = false;
            on_platform = true;
        }

        // Clear and render
        renderer.Clear();
        renderer.Copy(background, NullOpt, NullOpt);

        // Render platforms
        // Left platform
        for (int i = 0; i < 6; ++i) {
            renderer.Copy(tablas, NullOpt,
                         Rect(i * cell_width, left_platform.y, cell_width, cell_height));
        }

        // Right platform
        for (int i = 0; i < 6; ++i) {
            renderer.Copy(tablas, NullOpt,
                         Rect(i * cell_width + right_platform.x, right_platform.y,
                              cell_width, cell_height));
        }

        // Determine sprite frame
        int src_x = 0, src_y = 0;
        if (is_jumping) {
            src_x = 320;
            src_y = 0;
        } else if (is_running) {
            src_x = 64 + 64 * run_phase;
            src_y = 0;
        }
        if (agachado) {
            src_x = 255;
            src_y = 129;
        }

        // Render duck
        if (looking_right) {
            renderer.Copy(sprites, Rect(src_x, src_y, 64, 64),
                         Rect((int)position, (int)y_position, 64, 64), 0.0);
        } else {
            renderer.Copy(sprites, Rect(src_x, src_y, 64, 64),
                         Rect((int)position, (int)y_position, 64, 64), 0.0, NullOpt,
                         SDL_FLIP_HORIZONTAL);
        }

        // Debug text
        std::string text = "Position: " + std::to_string((int)position) +
                          ", Y: " + std::to_string((int)y_position) +
                          ", Velocity: " + std::to_string(vertical_velocity) +
                          ", On Platform: " + (on_platform ? "true" : "false");

        Texture text_sprite(renderer, font.RenderText_Blended(text, SDL_Color{255, 255, 255, 255}));
        renderer.Copy(text_sprite, NullOpt,
                     Rect(0, 0, text_sprite.GetWidth(), text_sprite.GetHeight()));

        renderer.Present();

        SDL_Delay(1);
    }

    return 0;
} catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
    return 1;
}