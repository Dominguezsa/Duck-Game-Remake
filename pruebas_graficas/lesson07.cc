#include <algorithm>
#include <exception>
#include <iostream>
#include <string>

#include <SDL2/SDL.h>
#include <SDL2pp/SDL2pp.hh>


using namespace SDL2pp;

int main() try {
    // Initialize SDL library
    SDL sdl(SDL_INIT_VIDEO);

    // Initialize SDL_ttf library
    SDLTTF ttf;

    // Create main window: 640x480 dimensions, resizable, "SDL2pp demo" title
    Window window("SDL2pp demo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 1200, 700,
                  SDL_WINDOW_RESIZABLE);
    // el eje x se divide en 12 celdas y el eje y en 14 celdas

    // Create accelerated video renderer with default driver
    Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load sprites image as a new texture
    Texture sprites(renderer, Surface("data/imagenesDePatos.png").SetColorKey(true, 0));

    // Enable alpha blending for the sprites
    sprites.SetBlendMode(SDL_BLENDMODE_BLEND);

    // Load background image outside of the loop
    Texture background(renderer, Surface("data/fondo.png"));
    // Agrega esto al inicio para cargar las imágenes de las tablas

    Texture tablas(renderer, Surface("data/tablon1.png"));


    // Load font, 12pt size
    Font font("data/Vera.ttf", 12);

    // Game state variables
    bool is_running = false;
    int run_phase = -1;
    float position = 0.0;
    bool looking_right = true;
    bool shift = false;
    bool agachado = false;
    int vcenter = renderer.GetOutputHeight() / 2;  // Y coordinate of window center

    unsigned int prev_ticks = SDL_GetTicks();

    // Main loop
    while (1) {
        unsigned int frame_ticks = SDL_GetTicks();
        unsigned int frame_delta = frame_ticks - prev_ticks;
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
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shift = true;
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
                    case SDLK_LSHIFT:
                    case SDLK_RSHIFT:
                        shift = false;
                        break;
                }
            }
        }

        // Update game state
        if (is_running && !agachado) {
            if (looking_right) {
                position += frame_delta * 0.2 * (shift ? 3 : 1);
            } else {
                position -= frame_delta * 0.2 * (shift ? 3 : 1);
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


        bool esta_cayendo = false;
        if (position > renderer.GetOutputWidth() / 2) {
            esta_cayendo = true;
        }
        if (vcenter >= renderer.GetOutputHeight() / 2 + 150) {
            esta_cayendo = false;
        }
        if (esta_cayendo) {
            vcenter += 5;
        }


        // Clear screen
        renderer.Clear();

        // Render the background (before sprites)
        renderer.Copy(background, NullOpt, NullOpt);

        // Render the tablas below the horizontal center line
        int tablas_height = renderer.GetOutputHeight() / 2;
        int tablas_width = renderer.GetOutputWidth() / 2;

        // Calculate the width and height of each cell
        int cell_width = renderer.GetOutputWidth() / 12;
        int cell_height = renderer.GetOutputHeight() / 14;

        // Render tablas in each cell
        for (int i = 0; i < 6; ++i) {
            renderer.Copy(tablas, NullOpt,
                          Rect(i * cell_width, tablas_height, cell_width, cell_height));
        }

        for (int i = 0; i < 6; ++i) {
            renderer.Copy(tablas, NullOpt,
                          Rect(i * cell_width + tablas_width, tablas_height + cell_height *3,
                               cell_width, cell_height));
        }

        // Pick sprite from sprite atlas based on running state
        int src_x = 0, src_y = 0;
        if (is_running) {
            src_x = 64 + 64 * run_phase;
            src_y = 0;
        }
        if (agachado) {
            src_x = 255;
            src_y = 129;
        }


        if (looking_right) {
            renderer.Copy(sprites, Rect(src_x, src_y, 64, 64),
                          Rect((int)position, vcenter - 64, 64, 64), 0.0);
        } else {
            renderer.Copy(sprites, Rect(src_x, src_y, 64, 64),
                          Rect((int)position, vcenter - 64, 64, 64), 0.0, NullOpt,
                          SDL_FLIP_HORIZONTAL);
        }

        // Create text string to render
        std::string text = "Position: " + std::to_string((int)position) +
                           ", running: " + (is_running ? "true" : "false") +
                           "vcenter: " + std::to_string(vcenter);

        // Render the text into new texture. Note that SDL_ttf render
        // text into Surface, which is converted into texture on the fly
        Texture text_sprite(renderer, font.RenderText_Blended(text, SDL_Color{255, 255, 255, 255}));

        // Copy texture into top-left corner of the window
        renderer.Copy(text_sprite, NullOpt,
                      Rect(0, 0, text_sprite.GetWidth(), text_sprite.GetHeight()));

        // Show rendered frame
        renderer.Present();

        // Frame limiter: sleep for a little bit to not eat 100% of CPU
        SDL_Delay(1);
    }

    // Here all resources are automatically released and libraries deinitialized
    return 0;
} catch (std::exception& e) {
    // If case of error, print it and exit with error
    std::cerr << e.what() << std::endl;
    return 1;
}
