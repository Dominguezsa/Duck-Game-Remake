#include "ScreenRenderer.h"

#include <iostream>
#include <string>
#include <utility>
#include <vector>

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
    // std::vector<SDL2pp::Rect> frameDucksArms = animationHelper.get_animation_frames(it);

    int arm_position_x;
    int arm_position_y;

    // Now lets render the ducks, taking into account the direction they are facing
    for (int i = 0; i < (int)ducks.size(); i++) {
        // std::cout << "Color for this duck id: " << +ducks[i].duck_id
        //           << " is: " << colors_per_id[ducks[i].duck_id] << std::endl;
        // std::cout << "Duck gun for id: " << +ducks[i].duck_id << " is: " << ducks[i].weapon.name
                  //<< std::endl;
        if (ducks[i].looking == 0) {
            arm_position_x = ducks[i].position.x + (DUCK_WIDTH * DUCK_SCALE) / 2.9;
            arm_position_y = ducks[i].position.y + (DUCK_HEIGTH * DUCK_SCALE) / 2.2;

            renderer.Copy(*resourceManager.getTexture(colors_per_id[ducks[i].duck_id]),
                          frameDucks[i].first,
                          SDL2pp::Rect(ducks[i].position.x, ducks[i].position.y,
                                       DUCK_WIDTH * DUCK_SCALE, DUCK_HEIGTH * DUCK_SCALE),
                          0.0, SDL2pp::NullOpt, SDL_FLIP_HORIZONTAL);
            // drawing the arms
            renderer.Copy(*resourceManager.getTexture(colors_per_id[ducks[i].duck_id]),
                          frameDucks[i].second,
                          SDL2pp::Rect(arm_position_x, arm_position_y, DUCK_ARM_WIDTH * DUCK_SCALE,
                                       DUCK_ARM_HEIGTH * DUCK_SCALE),
                          0.0, SDL2pp::NullOpt, SDL_FLIP_HORIZONTAL);

        } else {

            arm_position_x = ducks[i].position.x + (DUCK_ARM_WIDTH * DUCK_SCALE) / 3.5;
            arm_position_y = ducks[i].position.y + (DUCK_HEIGTH * DUCK_SCALE) / 2.2;

            renderer.Copy(*resourceManager.getTexture(colors_per_id[ducks[i].duck_id]),
                          frameDucks[i].first,
                          SDL2pp::Rect(ducks[i].position.x, ducks[i].position.y,
                                       DUCK_WIDTH * DUCK_SCALE, DUCK_HEIGTH * DUCK_SCALE));
            renderer.Copy(*resourceManager.getTexture(colors_per_id[ducks[i].duck_id]),
                          frameDucks[i].second,
                          SDL2pp::Rect(arm_position_x, arm_position_y, DUCK_ARM_WIDTH * DUCK_SCALE,
                                       DUCK_ARM_HEIGTH * DUCK_SCALE));
        }
    }
}


void ScreenRenderer::copyGuns(const std::vector<Duck>& ducks) {
    for (int i = 0; i < (int)ducks.size(); i++) {
        if (ducks[i].looking == 0) {
            renderer.Copy(*resourceManager.getTexture(ducks[i].weapon.name),
                          SDL2pp::NullOpt,
                          SDL2pp::Rect(ducks[i].position.x + (DUCK_WIDTH * DUCK_SCALE) / 2.9,
                                       ducks[i].position.y + (DUCK_HEIGTH * DUCK_SCALE) / 2.2,
                                       DUCK_ARM_WIDTH * DUCK_SCALE, DUCK_ARM_HEIGTH * DUCK_SCALE),
                          0.0, SDL2pp::NullOpt, SDL_FLIP_HORIZONTAL);
        } else {
            renderer.Copy(*resourceManager.getTexture("gun"),
                          SDL2pp::NullOpt,
                          SDL2pp::Rect(ducks[i].position.x + (DUCK_ARM_WIDTH * DUCK_SCALE) / 3.5,
                                       ducks[i].position.y + (DUCK_HEIGTH * DUCK_SCALE) / 2.2,
                                       DUCK_ARM_WIDTH * DUCK_SCALE, DUCK_ARM_HEIGTH * DUCK_SCALE));
        }
    }
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
                " Duck " + std::to_string(i + 1) +
                " is gliding: " + std::to_string(ducks[i].is_gliding) + " Duck " +
                std::to_string(i + 1) + " is on air: " + std::to_string(ducks[i].in_air) +
                " Duck " + std::to_string(i + 1) +
                " vertical velocity: " + std::to_string(ducks[i].vertical_velocity);

        SDL2pp::Texture text_sprite(renderer,
                                    resourceManager.getFont("vera")->RenderText_Blended(
                                            duck_position_text, SDL_Color{255, 255, 255, 255}));

        renderer.Copy(text_sprite, SDL2pp::NullOpt,
                      SDL2pp::Rect(0, i * 20, text_sprite.GetWidth(), text_sprite.GetHeight()));
    }
}
// Por ahora que reciba los dos patos por parámetro, probablemente en un futuro sean o una clase o
// directamente un vector de patos para la partida
void ScreenRenderer::updateScreen(const std::vector<Duck>& ducks, const int it) {
    renderer.Clear();
    copyBackground();
    copyPlatforms();
    copyDucks(ducks, it);
    //copyGuns(ducks);
    copyDebugText(ducks);

    renderer.Present();
}
