#ifndef DUCK_GRAPHIC_DATA_H
#define DUCK_GRAPHIC_DATA_H

#include <string>

#include <SDL2pp/SDL2pp.hh>

#include "../common/duck.h"

class DuckGraphicData {
private:
    Duck& my_duck;

public:
    // cppcheck-suppress unusedStructMember
    std::string current_animation;
    // cppcheck-suppress unusedStructMember
    int current_frame;
    // cppcheck-suppress unusedStructMember
    int animation_start;


    explicit DuckGraphicData(Duck& my_duck);

    void update_current_frame(int it);
    void update_current_animation();
    void set_animation_start(int it);

    ~DuckGraphicData() = default;
};

#endif  // DUCK_GRAPHIC_DATA_H
