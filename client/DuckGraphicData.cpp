#include "DuckGraphicData.h"

#include <SDL2pp/SDL2pp.hh>

DuckGraphicData::DuckGraphicData(Duck& my_duck):
        my_duck(my_duck),
        current_animation("duck_running"),
        current_frame(-1),
        animation_start(-1) {}


void DuckGraphicData::update_current_frame(int it) {
    if (current_animation == "duck_running") {
        if (my_duck.is_running) {
            current_frame = ((it - animation_start) / 6) % 6;
        } else {
            current_frame = 0;
        }
        // std::cout << "The frame I draw should be: " << current_frame << std::endl;
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
        // std::cout << "The JUMPING frame i draw should be: " << current_frame << std::endl;
    }
}

void DuckGraphicData::update_current_animation() {
    // std::cout << "The duck is jumping: " << my_duck.is_jumping << std::endl;
    if (my_duck.is_jumping) {
        current_animation = "duck_jumping";
    } else {
        current_animation = "duck_running";
    }
    // std::cout << "The current animation is: " << current_animation << std::endl;
}

void DuckGraphicData::set_animation_start(int it) {
    if (my_duck.is_running && animation_start == -1) {
        animation_start = it;
    } else if (!my_duck.is_running && animation_start != -1) {
        animation_start = -1;
    }
    // std::cout << "The animation start is: " << animation_start << std::endl;
}
