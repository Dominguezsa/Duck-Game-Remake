#include "ScreenRenderer.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <SDL2/SDL_render.h>

#include "../common/snapshot.h"

#define DUCK_WIDTH 32
#define DUCK_HEIGTH 32
#define DUCK_SCALE 3

#define DUCK_ARM_WIDTH 16
#define DUCK_ARM_HEIGTH 16

ScreenRenderer::ScreenRenderer(SDL2pp::Renderer& renderer, ResourceManager& resourceManager,
                               AnimationHelper& animHelp):
        renderer(renderer), resourceManager(resourceManager), animationHelper(animHelp) {}

void ScreenRenderer::copyDucks(const std::vector<DuckState>& ducks, const int it) {

    std::vector<std::pair<SDL2pp::Rect, SDL2pp::Rect>> frameDucks =
            animationHelper.get_animation_frames(it);

    // Now lets render the ducks, taking into account the direction they are facing
    for (int i = 0; i < (int)ducks.size(); i++) {

        int flip = ducks[i].looking == 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
        int arm_position_x = (ducks[i].looking == 0) ?
                                     ducks[i].position.x + (DUCK_WIDTH * DUCK_SCALE) / 2.9 :
                                     ducks[i].position.x + (DUCK_ARM_WIDTH * DUCK_SCALE) / 3.5;
        int arm_position_y = ducks[i].position.y + (DUCK_HEIGTH * DUCK_SCALE) / 2.2;

        SDL2pp::Texture& duck_texture =
                *resourceManager.getTexture(colors_per_id[ducks[i].duck_id]);

        SDL2pp::Rect duck_frame = frameDucks[i].first;
        SDL2pp::Rect arm_frame = frameDucks[i].second;

        // Rendering the duck
        renderer.Copy(duck_texture, duck_frame,
                      SDL2pp::Rect(ducks[i].position.x, ducks[i].position.y,
                                   DUCK_WIDTH * DUCK_SCALE, DUCK_HEIGTH * DUCK_SCALE),
                      0.0, SDL2pp::NullOpt, flip);

        // Rendering it's arms
        if (ducks[i].weapon == WeaponType::NoneType) {
            renderer.Copy(duck_texture, arm_frame,
                          SDL2pp::Rect(arm_position_x, arm_position_y, DUCK_ARM_WIDTH * DUCK_SCALE,
                                       DUCK_ARM_HEIGTH * DUCK_SCALE),
                          0.0, SDL2pp::NullOpt, flip);
        } else {
            // std::cout << "Trying to render the weapon\n";
            copyGun(ducks[i]);
        }
    }
}

void ScreenRenderer::copyGun(const DuckState& duck) {
    SDL2pp::Texture& weapon_texture = *resourceManager.getTexture(weapons_by_enum[duck.weapon]);
    int flip = duck.looking == 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    int x_position = duck.looking == 0 ? duck.position.x + (DUCK_ARM_WIDTH * DUCK_SCALE) / 3.5 - 6 :
                                         duck.position.x + (DUCK_WIDTH * DUCK_SCALE) / 2.9;
    SDL2pp::Rect weapon_frame = animationHelper.get_weapon_rect(weapons_by_enum[duck.weapon]);
    renderer.Copy(weapon_texture, weapon_frame,
                  SDL2pp::Rect(x_position, duck.position.y + (DUCK_HEIGTH * DUCK_SCALE) / 2.2 - 7,
                               DUCK_ARM_WIDTH * DUCK_SCALE + 10, DUCK_ARM_HEIGTH * DUCK_SCALE + 10),
                  0.0, SDL2pp::NullOpt, flip);
}


void ScreenRenderer::copyPlatforms(std::vector<Platform> platforms) {
    for (auto plat: platforms) {
        SDL2pp::Texture& platform_texture = *resourceManager.getTexture("tablas");
        SDL2pp::Rect platform_frame = resourceManager.getPlatformRect(platforms_by_enum[plat.id]);
        renderer.Copy(platform_texture, platform_frame,
                      SDL2pp::Rect(static_cast<int>(plat.x), static_cast<int>(plat.y) + 30,
                                   static_cast<int>(plat.width), static_cast<int>(plat.height)));
    }
}

void ScreenRenderer::copyBackground() {
    SDL2pp::Texture& background_texture = *resourceManager.getTexture("background");
    renderer.Copy(background_texture, SDL2pp::NullOpt, SDL2pp::NullOpt);
}


void ScreenRenderer::copyDebugText(const std::vector<DuckState>& ducks) {
    // Debug text
    for (int i = 0; i < (int)ducks.size(); ++i) {
        std::string duck_position_text =
                "Position of duck_" + std::to_string(i + 1) + ": " +
                std::to_string((int)ducks[i].position.x) + ", " +
                std::to_string((int)ducks[i].position.y) + " Duck " + std::to_string(i + 1) +
                " is running: " + std::to_string(ducks[i].is_running) + " Duck " +
                std::to_string(i + 1) + " is facing: " + std::to_string(ducks[i].looking) +
                " Duck " + std::to_string(i + 1) +
                " is jumping: " + std::to_string(ducks[i].is_jumping) + " Duck " +
                std::to_string(i + 1) + " is falling: " + std::to_string(ducks[i].is_falling) +
                " Duck " + std::to_string(i + 1) +
                " is ducking: " + std::to_string(ducks[i].is_ducking) + " Duck " +
                std::to_string(i + 1) + " is sliding: " + std::to_string(ducks[i].is_sliding) +
                " Duck's Weapon is: " + std::to_string(ducks[i].weapon);
        SDL2pp::Texture text_sprite(renderer,
                                    resourceManager.getFont("vera")->RenderText_Blended(
                                            duck_position_text, SDL_Color{255, 255, 255, 255}));

        renderer.Copy(text_sprite, SDL2pp::NullOpt,
                      SDL2pp::Rect(0, i * 20, text_sprite.GetWidth(), text_sprite.GetHeight()));
    }
}

void ScreenRenderer::copyWeapons(const std::vector<Weapon>& weapons) {
    for (int i = 0; i < (int)weapons.size(); i++) {
        SDL2pp::Texture& weapon_texture =
                *resourceManager.getTexture(weapons_by_enum[weapons[i].id]);

        SDL2pp::Rect weapon_frame = animationHelper.get_weapon_rect(weapons_by_enum[weapons[i].id]);
        renderer.Copy(weapon_texture, weapon_frame,
                      SDL2pp::Rect(weapons[i].pos.x, weapons[i].pos.y + 25, weapon_frame.w * 2.5,
                                   weapon_frame.h * 2.5));
    }
}

void ScreenRenderer::copyBullets(const std::vector<Bullet>& bullets) {

    // std::cout << "I have to keep drawing " << bullets.size() << " bullets\n";

    for (const auto& bullet: bullets) {
        // if (bullet.id == WeaponType::Sniper) {
        //     continue;
        // }

        SDL2pp::Texture& bullet_texture =
                (bullet.id == WeaponType::Sniper) ?
                        *resourceManager.getTexture("ak47") :
                        *resourceManager.getTexture(bullets_by_enum[bullet.id]);

        std::pair<SDL2pp::Rect, SDL2pp::Rect> bullet_info =
                animationHelper.get_bullet_rect(bullets_by_enum[bullet.id]);

        SDL2pp::Rect bullet_frame = bullet_info.first;
        SDL2pp::Rect bullet_rect = bullet_info.second;

        int angle = bullet.angle;

        if (!bullet.going_right) {
            angle = 180 - bullet.angle;
        }

        renderer.Copy(bullet_texture, bullet_frame,
                      SDL2pp::Rect(bullet.x, bullet.y, bullet_rect.w, bullet_rect.h), angle,
                      SDL2pp::NullOpt, SDL_FLIP_NONE);
    }
}

void ScreenRenderer::copy_lines() {
    int screen_width = 1250;
    int screen_height = 750;
    int num_lines_x = 25;
    int num_lines_y = 15;

    int line_spacing_x = screen_width / num_lines_x;
    int line_spacing_y = screen_height / num_lines_y;

    // Dibujar líneas verticales
    for (int i = 0; i <= num_lines_x; ++i) {
        int x = i * line_spacing_x;
        SDL_RenderDrawLine(renderer.Get(), x, 0, x, screen_height);
    }

    // Dibujar líneas horizontales
    for (int i = 0; i <= num_lines_y; ++i) {
        int y = i * line_spacing_y;
        SDL_RenderDrawLine(renderer.Get(), 0, y, screen_width, y);
    }
}


void ScreenRenderer::updateScreen(const Snapshot& snapshot, const int it) {
    renderer.Clear();
    copyBackground();
    copyPlatforms(snapshot.platforms);
    // copy_lines();
    copyDucks(snapshot.ducks, it);  // aca tambien se copian las armas si las portan los patos
    // copyDebugText(snapshot.ducks);
    copyWeapons(snapshot.weapons);  // aca se copian las armas que estan en el suelo para pickear
    copyBullets(snapshot.bullets);
    renderer.Present();
}

void ScreenRenderer::show_next_round() {
    renderer.Clear();
    copyBackground();
    SDL2pp::Texture& next_round_texture = *resourceManager.getTexture("next_round");
    renderer.Copy(next_round_texture, SDL2pp::Rect(0, 0, 750, 750),
                  SDL2pp::Rect(400, 150, 400, 400));
    renderer.Present();
}
