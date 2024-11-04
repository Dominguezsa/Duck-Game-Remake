#ifndef ANIMATION_HELPER_H
#define ANIMATION_HELPER_H

#include <vector>

#include "../common/duck.h"

class AnimationHelper {
private:
    // Esto debería ser un vector de patos, y probablemente tenga un método que sea update
    // animations o algo así que recorra el vector y actualice las animaciones, y los datos de esas
    // animaciones deberían estar en un map asociado a la id del pato específico y queseyo, después
    // ver

    std::vector<Duck>& ducks;

    std::vector<int> run_anim_phase_ducks;
    std::vector<int> run_anim_start_ducks;
    std::vector<int> jump_anim_phase_ducks;
    std::vector<int> jump_anim_start_ducks;

public:
    explicit AnimationHelper(std::vector<Duck>& ducks);
    void update_run_anim_start_duck1(int it);

    std::vector<int> get_run_anim_phase_duck1(int it);
    ~AnimationHelper() = default;
};

#endif  // ANIMATION_HELPER_H
