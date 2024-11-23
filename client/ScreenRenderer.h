#ifndef SCREEN_RENDERER_H
#define SCREEN_RENDERER_H

#include <string>
#include <vector>

#include <SDL2pp/Renderer.hh>
#include <SDL2pp/SDL2pp.hh>

#include "../common/duck.h"
#include "../common/types/duck_state.h"
#include "../common/types/match_state.h"
#include "../common/types/weapon_type.h"

#include "AnimationHelper.h"
#include "ResourceManager.h"

class ScreenRenderer {
private:
    SDL2pp::Renderer& renderer;
    ResourceManager& resourceManager;
    AnimationHelper& animationHelper;
    std::vector<std::string> colors_per_id = {"white_duck", "orange_duck", "grey_duck",
                                              "yellow_duck"};
    std::vector<std::string> weapons_by_enum = {
            "Granade",      "Banana", "PewPewLaser", "LaserRifle", "ak47",    "DuelPistol",
            "CowboyPistol", "Magnum", "Shotgun",     "Sniper",     "NoneType"};


    void copyBackground();
    void copyPlatforms();
    void copyDucks(const std::vector<DuckState>& ducks, const int it);
    void copyGun(const DuckState& duck);
    void copyDebugText(const std::vector<DuckState>& ducks);

public:
    ScreenRenderer(SDL2pp::Renderer& renderer, ResourceManager& resourceManager,
                   AnimationHelper& animHelp);
    void updateScreen(const Snapshot& snapshot, const int it);
};


#endif
