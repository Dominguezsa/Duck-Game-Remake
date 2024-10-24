#include "ResourceManager.h"

ResourceManager::ResourceManager(SDL2pp::Renderer& renderer): renderer(renderer) {}

// void ResourceManager::loadSFX() {

//     sfx.emplace("boom1", SDL2pp::Chunk("./data/audio/sfx/boom1.wav"));
//     sfx.emplace("boom2", SDL2pp::Chunk("./data/audio/sfx/boom2.wav"));
//     sfx.emplace("boom3", SDL2pp::Chunk("./data/audio/sfx/boom3.wav"));
// }

void ResourceManager::loadMusic() {

    music.emplace("back_music", std::make_shared<SDL2pp::Music>(
                                        "./data/audio/ost/back_music_space_mystery_out.ogg"));
}

// void ResourceManager::loadFonts() {
//     fonts.emplace("vera", SDL2pp::Font("./data/Vera.ttf", 12));
// }

// void ResourceManager::loadSprites() {

//     textures.emplace("duck", SDL2pp::Texture(renderer,
//     SDL2pp::Surface("data/imagenesDePatos.png").SetColorKey(true, 0)));
//     textures["duck"].SetBlendMode(SDL_BLENDMODE_BLEND);

//     textures.emplace("background", SDL2pp::Texture(renderer, SDL2pp::Surface("data/fondo.png")));
//     textures.emplace("tablas", SDL2pp::Texture(renderer, SDL2pp::Surface("data/tablon1.png")));
// }

void ResourceManager::loadResources() {
    // loadSFX();
    loadMusic();
    // loadFonts();
    // loadSprites();
}

std::shared_ptr<SDL2pp::Music> ResourceManager::getMusicTrack(const std::string& key) {

    auto it = music.find(key);
    return it->second;
}

ResourceManager::~ResourceManager() {}
