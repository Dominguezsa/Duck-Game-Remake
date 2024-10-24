#include "ResourceManager.h"

ResourceManager::ResourceManager(SDL2pp::Renderer& renderer): renderer(renderer) {}

void ResourceManager::loadSFX() {

    sfx["boom1"] = SDL2pp::Chunk("./data/audio/sfx/boom1.wav");
    sfx["boom2"] = SDL2pp::Chunk("./data/audio/sfx/boom2.wav");
    sfx["boom3"] = SDL2pp::Chunk("./data/audio/sfx/boom3.wav");
}

void ResourceManager::loadMusic() {

    music["back_music_space_mystery_out"] =
            SDL2pp::Music("./data/audio/ost/back_music_space_mystery_out.ogg");
}

void ResourceManager::loadFonts() { fonts["arial"] = SDL2pp::Font("./data/Vera.ttf", 12); }

void ResourceManager::loadSprites() {

    textures["duck"] = SDL2pp::Texture(
            renderer, SDL2pp::Surface("data/imagenesDePatos.png").SetColorKey(true, 0));
    textures["duck"].SetBlendMode(SDL_BLENDMODE_BLEND);
    textures["background"] = SDL2pp::Texture(renderer, SDL2pp::Surface("data/fondo.png"));
    textures["tablas"] = SDL2pp::Texture(renderer, SDL2pp::Surface("data/tablon1.png"));
}

void ResourceManager::loadResources() {
    loadSFX();
    loadMusic();
    loadFonts();
    loadSprites();
}

ResourceManager::~ResourceManager() {}
