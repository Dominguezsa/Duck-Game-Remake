#include "AnimationHelper.h"

#include <utility>

#include "ResourceManager.h"

AnimationHelper::AnimationHelper(ResourceManager& resourceManager):
        ducks_animation_data(), resourceManager(resourceManager) {

    // std::transform(ducks.begin(), ducks.end(), std::back_inserter(ducks_animation_data),
    //                [](Duck& duck) { return DuckGraphicData(duck); });
}

void AnimationHelper::loadDucks(std::vector<Duck>& ducks) {
    std::transform(ducks.begin(), ducks.end(), std::back_inserter(ducks_animation_data),
                   [](Duck& duck) { return DuckGraphicData(duck); });
}

void AnimationHelper::set_run_anim_start(int it) {
    for (auto& duck: ducks_animation_data) {
        duck.set_animation_start(it);
    }
}

void AnimationHelper::update_animation_frame(int it) {
    for (auto& duck: ducks_animation_data) {
        duck.update_current_animation();
        duck.update_current_frame(it);
    }
}

std::vector<std::pair<SDL2pp::Rect, SDL2pp::Rect>> AnimationHelper::get_animation_frames(int it) {

    // set_run_anim_start(it);
    update_animation_frame(it);

    std::vector<std::pair<SDL2pp::Rect, SDL2pp::Rect>> frames;

    // I prefer a normal for loop but cppcheck doesn't like it so whatever

    // std::cout << "Trying to get animation frames\n";
    // std::cout << "The size of the ducks animation data is: " << ducks_animation_data.size() <<
    // std::endl;
    std::transform(ducks_animation_data.begin(), ducks_animation_data.end(),
                   std::back_inserter(frames), [this](const DuckGraphicData& duck) {
                       return std::make_pair(
                               resourceManager.getAnimationFrame(duck.current_animation,
                                                                 duck.current_frame),
                               resourceManager.getAnimationFrame(duck.current_arm_animation,
                                                                 duck.current_frame));
                   });


    return frames;
}
