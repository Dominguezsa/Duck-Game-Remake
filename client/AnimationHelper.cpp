#include "AnimationHelper.h"

AnimationHelper::AnimationHelper(std::vector<Duck>& ducks):
        ducks(ducks),
        run_anim_phase_ducks({-1, -1}),
        run_anim_start_ducks({-1, -1}),
        jump_anim_phase_ducks({-1, -1}),
        jump_anim_start_ducks({-1, -1}) {}

void AnimationHelper::update_run_anim_start_duck1(int it) {
    for (int i = 0; i < (int)ducks.size(); i++) {
        if (ducks[i].is_running && run_anim_phase_ducks[i] == -1) {
            run_anim_start_ducks[i] = it;
        } else if (!ducks[i].is_running && run_anim_start_ducks[i] != -1) {
            run_anim_start_ducks[i] = -1;
        }
    }
}


// Again, repeating code but in a different place this time
std::vector<int> AnimationHelper::get_run_anim_phase_duck1(int it) {
    for (int i = 0; i < (int)ducks.size(); i++) {
        if (ducks[i].is_running) {
            if (run_anim_start_ducks[i] == -1) {
                run_anim_phase_ducks[i] = 0;
            } else {
                run_anim_phase_ducks[i] = (it - run_anim_start_ducks[i]) % 6;
            }
        }
    }
    return run_anim_phase_ducks;
}
