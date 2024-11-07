#include "AnimationHelper.h"

#include <utility>

#include "ResourceManager.h"

AnimationHelper::AnimationHelper(std::vector<Duck>& ducks, ResourceManager& resourceManager):
        ducks_animation_data(), resourceManager(resourceManager) {

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

    set_run_anim_start(it);
    update_animation_frame(it);

    std::vector<std::pair<SDL2pp::Rect, SDL2pp::Rect>> frames;

    // I prefer a normal for loop but cppcheck doesn't like it so whatever

    for (const auto& duck: ducks_animation_data) {
        // cppcheck-suppress useStlAlgorithm
        frames.push_back(std::make_pair(
                resourceManager.getAnimationFrame(duck.current_animation, duck.current_frame),
                resourceManager.getAnimationFrame(duck.current_arm_animation, duck.current_frame)));
    }

    // std::transform(ducks_animation_data.begin(), ducks_animation_data.end(),
    //                std::back_inserter(frames), [this](const DuckGraphicData& duck) {
    //                    return resourceManager.getAnimationFrame(duck.current_animation,
    //                                                             duck.current_frame);
    //                });

    // for (auto& duck: ducks_animation_data) {
    //     frames.push_back(
    //             resourceManager.getAnimationFrame(duck.current_animation, duck.current_frame));
    // }


    return frames;
}

// std::vector<SDL2pp::Rect> AnimationHelper::get_animation_arms(int it) {

//     std::vector<SDL2pp::Rect> frames;

//     std::transform(ducks_animation_data.begin(), ducks_animation_data.end(),
//                    std::back_inserter(frames), [this](const DuckGraphicData& duck) {
//                        return resourceManager.getAnimationFrame(duck.current_arm_animation,
//                                                                 duck.current_frame);
//                    });

//     // for (auto& duck: ducks_animation_data) {
//     //     frames.push_back(
//     //             resourceManager.getAnimationFrame(duck.current_arm_animation,
//     duck.current_frame));
//     // }

//     return frames;
// }
