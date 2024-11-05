#include "ScreenRenderer.h"

#include <string>
#include <vector>

ScreenRenderer::ScreenRenderer(SDL2pp::Renderer& renderer, ResourceManager& resourceManager,
                               AnimationHelper& animHelp):
        renderer(renderer), resourceManager(resourceManager), animationHelper(animHelp) {}

void ScreenRenderer::copyDucks(const std::vector<Duck>& ducks, const int it) {
    // Esto es código repetido pero un toque más maquillado, osea es pasable pero no

    std::vector<SDL2pp::Rect> frameDucks;

    animationHelper.update_run_anim_start(it);

    std::vector<int> anim_phase_ducks = animationHelper.get_run_anim_phase(it);

    frameDucks = resourceManager.getAnimationFrame("duck_running", anim_phase_ducks);


    // Now lets render the ducks, taking into account the direction they are facing

    for (int i = 0; i < (int)ducks.size(); i++) {
        std::string color;
        if (i % 2 == 0) {
            color = "white_duck";
        } else {
            color = "orange_duck";
        }
        if (ducks[i].looking == 0) {
            renderer.Copy(*resourceManager.getTexture(color), frameDucks[i],
                          SDL2pp::Rect(ducks[i].position.x, ducks[i].position.y, 62, 62), 0.0,
                          SDL2pp::NullOpt, SDL_FLIP_HORIZONTAL);
        } else {
            renderer.Copy(*resourceManager.getTexture(color), frameDucks[i],
                          SDL2pp::Rect(ducks[i].position.x, ducks[i].position.y, 62, 62));
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
    copyDebugText(ducks);

    renderer.Present();
}
