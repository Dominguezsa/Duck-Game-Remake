#include "ResourceManager.h"

#include <memory>
#include <vector>

#include <SDL2pp/Rect.hh>

#define DUCK_RECT_WIDTH 32
#define DUCK_RECT_HEIGHT 32
#define DUCK_RECT_X 1
#define DUCK_RECT_Y 7

#define DUCK_RUNNING_FRAMES 6
#define DUCK_JUMPING_FRAMES 6
#define DUCK_RUNNING_ARMS_FRAMES 6

#define DUCK_JUMPING_ANIM_X 1
#define DUCK_JUMPING_ANIM_Y 39

#define DUCK_RUNNING_ARMS_X 1
#define DUCK_RUNNING_ARMS_Y 518
#define DUCK_RUNNING_WIDTH 16
#define DUCK_RUNNING_HEIGHT 16

ResourceManager::ResourceManager(SDL2pp::Renderer& renderer): renderer(renderer) {}

void ResourceManager::loadSFX() {

    sfx.emplace("boom1", std::make_shared<SDL2pp::Chunk>("../client/data/audio/sfx/boom1.wav"));
    sfx.emplace("boom2", std::make_shared<SDL2pp::Chunk>("../client/data/audio/sfx/boom2.wav"));
    sfx.emplace("boom3", std::make_shared<SDL2pp::Chunk>("../client/data/audio/sfx/boom3.wav"));
    std::cout << "Every sfx loaded correctly\n";
}

void ResourceManager::loadMusic() {
    music.emplace("back_music",
                  std::make_shared<SDL2pp::Music>(
                          "../client/data/audio/ost/back_music_space_mystery_out.ogg"));
}

void ResourceManager::loadFonts() {
    fonts.emplace("vera", std::make_shared<SDL2pp::Font>("../client/data/Vera.ttf", 12));
}

void ResourceManager::loadSprites() {

    textures.emplace(
            "duck",
            std::make_shared<SDL2pp::Texture>(
                    renderer,
                    SDL2pp::Surface("../client/data/imagenesDePatos.png").SetColorKey(true, 0)));

    textures["duck"]->SetBlendMode(SDL_BLENDMODE_BLEND);

    textures.emplace("background", std::make_shared<SDL2pp::Texture>(
                                           renderer, SDL2pp::Surface("../client/data/fondo.png")));

    textures.emplace("tablas", std::make_shared<SDL2pp::Texture>(
                                       renderer, SDL2pp::Surface("../client/data/tablon1.png")));

    // Loading white duck sprite_sheet
    textures.emplace(
            "white_duck",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("../client/data/sprites/ducks/white_duck.png")));
    // Loading orange duck sprite_sheet
    textures.emplace(
            "orange_duck",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("../client/data/sprites/ducks/orange_duck.png")));

    std::cout << "All textures loaded correctly\n";
}

void ResourceManager::loadAnimationFrames() {

    // This is for the running animation
    std::vector<SDL2pp::Rect> duckFrames;

    for (int i = 0; i < DUCK_RUNNING_FRAMES; i++) {
        duckFrames.emplace_back(SDL2pp::Rect(DUCK_RECT_X + i * DUCK_RECT_WIDTH, DUCK_RECT_Y,
                                             DUCK_RECT_WIDTH, DUCK_RECT_HEIGHT));
    }

    animationFrames.emplace("duck_running", duckFrames);

    duckFrames.clear();

    // This is for the running arms animation
    for (int i = 0; i < DUCK_RUNNING_ARMS_FRAMES; i++) {
        duckFrames.emplace_back(SDL2pp::Rect(DUCK_RUNNING_ARMS_X + i * DUCK_RUNNING_WIDTH,
                                             DUCK_RUNNING_ARMS_Y, DUCK_RUNNING_WIDTH,
                                             DUCK_RUNNING_HEIGHT));
    }

    animationFrames.emplace("duck_running_arms", duckFrames);

    duckFrames.clear();

    for (int i = 0; i < DUCK_JUMPING_FRAMES; i++) {
        duckFrames.emplace_back(SDL2pp::Rect(DUCK_JUMPING_ANIM_X + i * DUCK_RECT_WIDTH,
                                             DUCK_JUMPING_ANIM_Y, DUCK_RECT_WIDTH,
                                             DUCK_RECT_HEIGHT));
    }

    animationFrames.emplace("duck_jumping", duckFrames);


    std::cout << "All animation frames loaded correctly\n";
}

void ResourceManager::loadResources() {
    loadSFX();
    loadMusic();
    loadFonts();
    loadSprites();
    loadAnimationFrames();
}

std::shared_ptr<SDL2pp::Music> ResourceManager::getMusicTrack(const std::string& key) {

    auto it = music.find(key);
    return it->second;
}

std::shared_ptr<SDL2pp::Font> ResourceManager::getFont(const std::string& key) {

    auto it = fonts.find(key);
    return it->second;
}

std::shared_ptr<SDL2pp::Chunk> ResourceManager::getSFX(const std::string& key) {

    auto it = sfx.find(key);
    return it->second;
}

std::shared_ptr<SDL2pp::Texture> ResourceManager::getTexture(const std::string& key) {

    auto it = textures.find(key);
    return it->second;
}

SDL2pp::Rect ResourceManager::getAnimationFrame(const std::string& key, const int frame) {
    return animationFrames[key][frame];
}

ResourceManager::~ResourceManager() {}
