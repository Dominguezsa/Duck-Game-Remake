#include "DuckGraphicData.h"

#include <SDL2pp/SDL2pp.hh>

DuckGraphicData::DuckGraphicData(DuckState& my_duck):
        my_duck(my_duck),
        current_animation("duck_running"),
        current_frame(-1),
        animation_start(-1),
        current_arm_animation("duck_running_arms") {}


void DuckGraphicData::update_current_frame(int it) {
    if (current_animation == "duck_running") {
        if (my_duck.is_running) {
            current_frame = ((it - animation_start) / 6) % 6;
        } else {
            current_frame = 0;
        }
    } else if (current_animation == "duck_jumping") {
        if (my_duck.is_falling) {
            current_frame = 3;
        } else {
            if (it - animation_start < 4) {
                current_frame = 0;
            } else {
                current_frame = 1;
            }
        }
    } else {
        current_frame = 0;
    }
}

void DuckGraphicData::update_current_animation() {
    if (!my_duck.is_alive) {
        current_animation = "duck_sliding";
        current_arm_animation = "empty";
        return;
    }
    if (my_duck.in_air && my_duck.is_sliding) {
        current_animation = "duck_sliding_air";
        current_arm_animation = "empty";
    } else if (my_duck.is_jumping) {
        current_animation = "duck_jumping";
        current_arm_animation = "duck_jumping_arms";
    } else if (my_duck.is_ducking) {
        current_animation = "duck_ducking";
        current_arm_animation = "empty";
    } else if (my_duck.is_sliding) {
        current_animation = "duck_sliding";
        current_arm_animation = "empty";
    } else {
        current_animation = "duck_running";
        current_arm_animation = "duck_running_arms";
    }
}

void DuckGraphicData::set_animation_start(int it) {
    animation_start = (animation_start == -1) ? it : -1;
}
