#ifndef SCREEN_RENDERER_H
#define SCREEN_RENDERER_H

#include <string>
#include <vector>

#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>

#include "../common/duck.h"
#include "../common/types/match_state.h"

#include "AnimationHelper.h"
#include "ResourceManager.h"

class ScreenRenderer {
private:
    SDL2pp::Renderer& renderer;
    ResourceManager& resourceManager;
    AnimationHelper& animationHelper;
    std::vector<std::string> colors_per_id = {"white_duck", "orange_duck", "grey_duck",
                                              "yellow_duck"};
    std::vector<std::string> weapons_by_enum = {"ak74", "coilgun", "magnum"};


    void copyBackground();
    void copyPlatforms();
    void copyDucks(const std::vector<Duck>& ducks, const int it);
    void copyDebugText(const std::vector<Duck>& ducks);
    void copyGuns(const std::vector<Duck>& ducks);

public:
    ScreenRenderer(SDL2pp::Renderer& renderer, ResourceManager& resourceManager,
                   AnimationHelper& animHelp);
    void updateScreen(const std::vector<Duck>& ducks, const int it);
};


#endif
