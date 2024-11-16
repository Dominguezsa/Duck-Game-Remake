#ifndef SCREEN_RENDERER_H
#define SCREEN_RENDERER_H

#include <string>
#include <vector>

#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>

#include "../common/duck.h"

#include "AnimationHelper.h"
#include "ResourceManager.h"

struct Platform {
    float x;
    float y;
    float width;
    float height;
};

class ScreenRenderer {
private:
    SDL2pp::Renderer& renderer;
    ResourceManager& resourceManager;
    AnimationHelper& animationHelper;
    std::vector<std::string> colors_per_id = {"white_duck", "orange_duck", "grey_duck",
                                              "yellow_duck"};

    void copyBackground();
    void copyPlatforms();
    void copyDucks(const std::vector<Duck>& ducks, const int it);
    void copyDuck();
    void copyGun(const Duck& duck);
    void copyDebugText(const std::vector<Duck>& ducks);

public:
    ScreenRenderer(SDL2pp::Renderer& renderer, ResourceManager& resourceManager,
                   AnimationHelper& animHelp);
    void updateScreen(const std::vector<Duck>& ducks, const int it);
};


#endif
