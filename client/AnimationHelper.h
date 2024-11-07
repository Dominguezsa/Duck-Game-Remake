#ifndef ANIMATION_HELPER_H
#define ANIMATION_HELPER_H

#include <algorithm>
#include <map>
#include <vector>

#include "../common/duck.h"

#include "DuckGraphicData.h"
#include "ResourceManager.h"

class AnimationHelper {
private:
    // cppcheck-suppress unusedStructMember
    std::vector<DuckGraphicData> ducks_animation_data;
    // cppcheck-suppress unusedStructMember
    std::vector<int> current_frames_per_duck;
    // cppcheck-suppress unusedStructMember
    ResourceManager& resourceManager;
    // std::map<uint8_t, AnimationData> duckAnimData;

public:
    explicit AnimationHelper(std::vector<Duck>& ducks, ResourceManager& resourceManager);
    void set_run_anim_start(int it);
    void update_animation_frame(int it);
    std::vector<SDL2pp::Rect> get_animation_frames(int it);
    ~AnimationHelper() = default;
};

#endif  // ANIMATION_HELPER_H
