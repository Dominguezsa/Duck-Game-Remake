#ifndef ANIMATION_HELPER_H
#define ANIMATION_HELPER_H

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

#include "../common/duck.h"

#include "DuckGraphicData.h"
#include "ResourceManager.h"

class AnimationHelper {
private:
    // Esto debería ser un vector de patos, y probablemente tenga un método que sea update
    // animations o algo así que recorra el vector y actualice las animaciones, y los datos de esas
    // animaciones deberían estar en un map asociado a la id del pato específico y queseyo, después
    // ver
    // Debería tener alguna estructura que tenga todos los datos de la animación de cada pato,
    // Tipo un struct animationData
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
    std::vector<std::pair<SDL2pp::Rect, SDL2pp::Rect>> get_animation_frames(int it);
    // std::vector<SDL2pp::Rect> get_animation_arms(int it);
    ~AnimationHelper() = default;
};

#endif  // ANIMATION_HELPER_H
