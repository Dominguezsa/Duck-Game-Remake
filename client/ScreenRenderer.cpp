#include "ScreenRenderer.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

#include <SDL2/SDL_render.h>

#define DUCK_WIDTH 32
#define DUCK_HEIGTH 32
#define DUCK_SCALE 3

#define DUCK_ARM_WIDTH 16
#define DUCK_ARM_HEIGTH 16

ScreenRenderer::ScreenRenderer(SDL2pp::Renderer& renderer, ResourceManager& resourceManager,
                               AnimationHelper& animHelp):
        renderer(renderer), resourceManager(resourceManager), animationHelper(animHelp) {}

void ScreenRenderer::copyDucks(const std::vector<Duck>& ducks, const int it) {

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
        if (ducks[i].weapon.name == "None") {
            renderer.Copy(duck_texture, arm_frame,
                          SDL2pp::Rect(arm_position_x, arm_position_y, DUCK_ARM_WIDTH * DUCK_SCALE,
                                       DUCK_ARM_HEIGTH * DUCK_SCALE),
                          0.0, SDL2pp::NullOpt, flip);
        } else {
            copyGun(ducks[i]);
        }
    }
}

void ScreenRenderer::copyGun(const Duck& duck) {
    SDL2pp::Texture& weapon_texture = *resourceManager.getTexture(duck.weapon.name);
    int flip = duck.looking == 0 ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
    int x_position = duck.looking == 0 ? duck.position.x + (DUCK_ARM_WIDTH * DUCK_SCALE) / 3.5 - 6 :
                                         duck.position.x + (DUCK_WIDTH * DUCK_SCALE) / 2.9;
    renderer.Copy(weapon_texture, SDL2pp::Rect(1, 19, 32, 32),
                  SDL2pp::Rect(x_position, duck.position.y + (DUCK_HEIGTH * DUCK_SCALE) / 2.2 - 7,
                               DUCK_ARM_WIDTH * DUCK_SCALE + 10, DUCK_ARM_HEIGTH * DUCK_SCALE + 10),
                  0.0, SDL2pp::NullOpt, flip);
}


void ScreenRenderer::copyPlatforms() {
    int cell_width = renderer.GetOutputWidth() / 12;
    int cell_height = renderer.GetOutputHeight() / 14;
    Platform left_platform = {0, (float)renderer.GetOutputHeight() / 2,
                              (float)renderer.GetOutputWidth() / 2, (float)cell_height};

    Platform right_platform = {(float)renderer.GetOutputWidth() / 2,
                               (float)renderer.GetOutputHeight() / 2 + cell_height * 3,
                               (float)renderer.GetOutputWidth() / 2, (float)cell_height};
    for (int i = 0; i < 6; ++i) {
        renderer.Copy(*resourceManager.getTexture("tablas"), SDL2pp::NullOpt,
                      SDL2pp::Rect(i * cell_width, left_platform.y, cell_width, cell_height));
    }

    // Right platform
    for (int i = 0; i < 6; ++i) {
        renderer.Copy(*resourceManager.getTexture("tablas"), SDL2pp::NullOpt,
                      SDL2pp::Rect(i * cell_width + right_platform.x, right_platform.y, cell_width,
                                   cell_height));
    }
}

void ScreenRenderer::copyBackground() {
    renderer.Copy(*resourceManager.getTexture("background"), SDL2pp::NullOpt, SDL2pp::NullOpt);
}

void ScreenRenderer::copyDebugText(const std::vector<Duck>& ducks) {
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
                // " Duck " + std::to_string(i + 1) +
                // " is gliding: " + std::to_string(ducks[i].is_gliding) + " Duck " +
                // std::to_string(i + 1) + " is on air: " + std::to_string(ducks[i].in_air) +
                // " Duck " + std::to_string(i + 1) +
                // " vertical velocity: " + std::to_string(ducks[i].vertical_velocity) +
                " Duck " + std::to_string(i + 1) +
                " is ducking: " + std::to_string(ducks[i].is_ducking) + " Duck " +
                std::to_string(i + 1) + " is sliding: " + std::to_string(ducks[i].is_sliding) +
                " Duck " + std::to_string(i + 1) +
                " horizontal velocity: " + std::to_string(ducks[i].horizontal_velocity);

        SDL2pp::Texture text_sprite(renderer,
                                    resourceManager.getFont("vera")->RenderText_Blended(
                                            duck_position_text, SDL_Color{255, 255, 255, 255}));

        renderer.Copy(text_sprite, SDL2pp::NullOpt,
                      SDL2pp::Rect(0, i * 20, text_sprite.GetWidth(), text_sprite.GetHeight()));
    }
}

void ScreenRenderer::updateScreen(const std::vector<Duck>& ducks, const int it) {
    renderer.Clear();
    copyBackground();
    copyPlatforms();
    copyDucks(ducks, it);
    // copyGuns(ducks);
    copyDebugText(ducks);

    renderer.Present();
}
