#include "AnimationHelper.h"

AnimationHelper::AnimationHelper(Duck& duck1, Duck& duck2): duck1(duck1), duck2(duck2) {}

void AnimationHelper::update_run_anim_start_duck1(int it) {
    if (duck1.is_running && run_anim_phase_duck1 == -1) {
        run_anim_start_duck1 = it;
    } else if (!duck1.is_running && run_anim_start_duck1 != -1) {
        run_anim_start_duck1 = -1;
    }
}

void AnimationHelper::update_run_anim_start_duck2(int it) {
    if (duck2.is_running && run_anim_phase_duck2 == -1) {
        run_anim_start_duck2 = it;
    } else if (!duck2.is_running && run_anim_start_duck2 != -1) {
        run_anim_start_duck2 = -1;
    }
}
// Again, repeating code but in a different place this time
int AnimationHelper::get_run_anim_phase_duck1(int it) {
    if (duck1.is_running) {
        run_anim_phase_duck1 = ((it - run_anim_start_duck1) / 6) % 6;
    } else {
        run_anim_phase_duck1 = 0;
    }
    return run_anim_phase_duck1;
}

int AnimationHelper::get_run_anim_phase_duck2(int it) {
    if (duck2.is_running) {
        run_anim_phase_duck2 = ((it - run_anim_start_duck2) / 6) % 6;
    } else {
        run_anim_phase_duck2 = 0;
    }
    return run_anim_phase_duck2;
}
