#ifndef SCREEN_RENDERER_H
#define SCREEN_RENDERER_H

#include <vector>

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
    void copyDucks(const std::vector<Duck>& ducks, const int it);
    void copyDebugText(const std::vector<Duck>& ducks);

public:
    ScreenRenderer(SDL2pp::Renderer& renderer, ResourceManager& resourceManager,
                   AnimationHelper& animHelp);
    void updateScreen(const std::vector<Duck>& ducks, const int it);
};


#endif
