#include "ResourceManager.h"

#include <memory>

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

    // textures.emplace("duck", SDL2pp::Texture(renderer,
    // SDL2pp::Surface("data/imagenesDePatos.png").SetColorKey(true, 0)));
    textures["duck"]->SetBlendMode(SDL_BLENDMODE_BLEND);

    textures.emplace("background", std::make_shared<SDL2pp::Texture>(
                                           renderer, SDL2pp::Surface("../client/data/fondo.png")));

    // textures.emplace("background", SDL2pp::Texture(renderer, SDL2pp::Surface("data/fondo.png")));
    textures.emplace("tablas", std::make_shared<SDL2pp::Texture>(
                                       renderer, SDL2pp::Surface("../client/data/tablon1.png")));
    // textures.emplace("tablas", SDL2pp::Texture(renderer, SDL2pp::Surface("data/tablon1.png")));
    std::cout << "All textures loaded correctly\n";
}

void ResourceManager::loadResources() {
    loadSFX();
    loadMusic();
    loadFonts();
    loadSprites();
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


ResourceManager::~ResourceManager() {}
