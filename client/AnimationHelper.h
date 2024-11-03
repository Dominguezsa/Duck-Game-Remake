#ifndef ANIMATION_HELPER_H
#define ANIMATION_HELPER_H

#include "../common/duck.h"

class AnimationHelper {
private:
    // Esto debería ser un vector de patos, y probablemente tenga un método que sea update
    // animations o algo así que recorra el vector y actualice las animaciones, y los datos de esas
    // animaciones deberían estar en un map asociado a la id del pato específico y queseyo, después
    // ver

    Duck& duck1;
    Duck& duck2;

    int run_anim_start_duck1 = -1;
    int run_anim_start_duck2 = -1;

    int run_anim_phase_duck1 = -1;
    int run_anim_phase_duck2 = -1;

    int jump_anim_phase_duck1 = -1;
    int jump_anim_phase_duck2 = -1;

    int jump_anim_start_duck1 = -1;
    int jump_anim_start_duck2 = -1;

public:
    AnimationHelper(Duck& duck1, Duck& duck2);
    void update_run_anim_start_duck1(int it);
    void update_run_anim_start_duck2(int it);

    int get_run_anim_phase_duck1(int it);
    int get_run_anim_phase_duck2(int it);
    ~AnimationHelper() = default;
};

#endif  // ANIMATION_HELPER_H
