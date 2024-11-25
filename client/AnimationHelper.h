#ifndef ANIMATION_HELPER_H
#define ANIMATION_HELPER_H

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

#include "../common/duck.h"

#include "../common/types/weapon_type.h"

#include "DuckGraphicData.h"
#include "ResourceManager.h"

class AnimationHelper {
private:
    std::vector<DuckGraphicData> ducks_animation_data;
    std::vector<int> current_frames_per_duck;
    ResourceManager& resourceManager;

public:
    explicit AnimationHelper(ResourceManager& resourceManager);
    void set_run_anim_start(int it);
    void update_animation_frame(int it);
    std::vector<std::pair<SDL2pp::Rect, SDL2pp::Rect>> get_animation_frames(int it);
    void loadDucks(std::vector<DuckState>& ducks);

    SDL2pp::Rect get_weapon_rect(const std::string& key);
    // std::vector<SDL2pp::Rect> get_animation_arms(int it);
    ~AnimationHelper() = default;
};

#endif  // ANIMATION_HELPER_H
