#include "AnimationHelper.h"

#include <string>
#include <utility>

#include "ResourceManager.h"

AnimationHelper::AnimationHelper(ResourceManager& resourceManager):
        ducks_animation_data(), resourceManager(resourceManager) {}

void AnimationHelper::loadDucks(std::vector<DuckState>& ducks) {
    std::transform(ducks.begin(), ducks.end(), std::back_inserter(ducks_animation_data),
                   [](DuckState& duck) { return DuckGraphicData(duck); });
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
// This returns a pair of frames, the first one is the duck frame and the second is the arms frame
// to sync both at the same time
std::vector<std::pair<SDL2pp::Rect, SDL2pp::Rect>> AnimationHelper::get_animation_frames(int it) {

    // set_run_anim_start(it);
    update_animation_frame(it);

    std::vector<std::pair<SDL2pp::Rect, SDL2pp::Rect>> frames;

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

SDL2pp::Rect AnimationHelper::get_weapon_rect(const std::string& key) {
    return resourceManager.getWeaponRect(key);
}

std::pair<SDL2pp::Rect, SDL2pp::Rect> AnimationHelper::get_bullet_rect(const std::string& key) {
    return resourceManager.getBulletRect(key);
}
