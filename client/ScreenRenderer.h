#ifndef SCREEN_RENDERER_H
#define SCREEN_RENDERER_H

#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>

#include "../common/duck.h"

#include "AnimationHelper.h"
#include "ResourceManager.h"

struct Platform {
    // cppcheck-suppress unusedStructMember
    float x;
    // cppcheck-suppress unusedStructMember
    float y;
    // cppcheck-suppress unusedStructMember
    float width;
    // cppcheck-suppress unusedStructMember
    float height;
};

class ScreenRenderer {
private:
    SDL2pp::Renderer& renderer;
    ResourceManager& resourceManager;
    AnimationHelper& animationHelper;

    void copyBackground();
    void copyPlatforms();
    void copyDucks(const DuckState& duck1, const DuckState& duck2, const int it);
    void copyDebugText(const DuckState& duck1, const DuckState& duck2);

public:
    ScreenRenderer(SDL2pp::Renderer& renderer, ResourceManager& resourceManager,
                   AnimationHelper& animHelp);
    void updateScreen(Duck& duck1, Duck& duck2, const int it);
};


#endif
