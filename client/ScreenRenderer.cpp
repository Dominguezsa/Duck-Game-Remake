#include "ScreenRenderer.h"

#include <string>

ScreenRenderer::ScreenRenderer(SDL2pp::Renderer& renderer, ResourceManager& resourceManager,
                               AnimationHelper& animHelp):
        renderer(renderer), resourceManager(resourceManager), animationHelper(animHelp) {}

void ScreenRenderer::copyDucks(const DuckState& duck1_state, const DuckState& duck2_state,
                               const int it) {
    // Esto es código repetido pero un toque más maquillado, osea es pasable pero no

    SDL2pp::Rect frameDuck1;
    SDL2pp::Rect frameDuck2;

    animationHelper.update_run_anim_start_duck1(it);
    animationHelper.update_run_anim_start_duck2(it);

    int anim_phase_duck1 = animationHelper.get_run_anim_phase_duck1(it);
    int anim_phase_duck2 = animationHelper.get_run_anim_phase_duck2(it);

    frameDuck1 = resourceManager.getAnimationFrame("duck_running", anim_phase_duck1);
    frameDuck2 = resourceManager.getAnimationFrame("duck_running", anim_phase_duck2);

    // Now lets render the ducks, taking into account the direction they are facing
    if (duck1_state.looking == 0) {
        renderer.Copy(*resourceManager.getTexture("white_duck"), frameDuck1,
                      SDL2pp::Rect(duck1_state.position.x, duck1_state.position.y, 62, 62), 0.0,
                      SDL2pp::NullOpt, SDL_FLIP_HORIZONTAL);
    } else {
        renderer.Copy(*resourceManager.getTexture("white_duck"), frameDuck1,
                      SDL2pp::Rect(duck1_state.position.x, duck1_state.position.y, 62, 62));
    }

    if (duck2_state.looking == 0) {
        renderer.Copy(*resourceManager.getTexture("orange_duck"), frameDuck2,
                      SDL2pp::Rect(duck2_state.position.x, duck2_state.position.y, 62, 62), 0.0,
                      SDL2pp::NullOpt, SDL_FLIP_HORIZONTAL);
    } else {
        renderer.Copy(*resourceManager.getTexture("orange_duck"), frameDuck2,
                      SDL2pp::Rect(duck2_state.position.x, duck2_state.position.y, 62, 62));
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

void ScreenRenderer::copyDebugText(const DuckState& duck1_state, const DuckState& duck2_state) {
    // Debug text

    std::string duck_position_text =
            "Position of duck_1: " + std::to_string((int)duck1_state.position.x) + ", " +
            std::to_string((int)duck1_state.position.y) + " Position of duck_2 " +
            std::to_string((int)duck2_state.position.x) + ", " +
            std::to_string((int)duck2_state.position.y) +
            " Duck 1 is running: " + std::to_string(duck1_state.is_running) +
            " Duck 2 is running: " + std::to_string(duck2_state.is_running) +
            " Duck 1 is facing: " + std::to_string(duck1_state.looking) +
            " Duck 2 is facing: " + std::to_string(duck2_state.looking);

    SDL2pp::Texture text_sprite(renderer,
                                resourceManager.getFont("vera")->RenderText_Blended(
                                        duck_position_text, SDL_Color{255, 255, 255, 255}));

    renderer.Copy(text_sprite, SDL2pp::NullOpt,
                  SDL2pp::Rect(0, 0, text_sprite.GetWidth(), text_sprite.GetHeight()));
}
// Por ahora que reciba los dos patos por parámetro, probablemente en un futuro sean o una clase o
// directamente un vector de patos para la partida
void ScreenRenderer::updateScreen(Duck& duck1, Duck& duck2, const int it) {

    DuckState duck1_state;
    DuckState duck2_state;

    duck1.get_state(duck1_state);
    duck2.get_state(duck2_state);

    renderer.Clear();

    copyBackground();
    copyPlatforms();
    copyDucks(duck1_state, duck2_state, it);
    copyDebugText(duck1_state, duck2_state);

    renderer.Present();
}
