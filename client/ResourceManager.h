#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include <vector>
#include "../common/types/weapon_type.h"

#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Font.hh>
#include <SDL2pp/Music.hh>
#include <SDL2pp/Texture.hh>
#include <unistd.h>

class ResourceManager {
private:
    std::map<std::string, std::shared_ptr<SDL2pp::Chunk>> sfx;
    std::map<std::string, std::shared_ptr<SDL2pp::Music>> music;
    std::map<std::string, std::shared_ptr<SDL2pp::Font>> fonts;
    std::map<std::string, std::shared_ptr<SDL2pp::Texture>> textures;
    std::map<std::string, std::vector<SDL2pp::Rect>> animationFrames;
    std::map<std::string, SDL2pp::Rect> weaponFrames;
    std::map<std::string, SDL2pp::Rect> bulletFrames;
    SDL2pp::Renderer& renderer;

public:
    explicit ResourceManager(SDL2pp::Renderer& renderer);
    ~ResourceManager();
    void loadResources(uint8_t playerAmount);
    void loadSFX();
    void loadMusic();
    void loadFonts();
    void loadSprites(uint8_t playerAmount);
    void loadAnimationFrames();
    void load_weapons_rect();
    void load_bullets_rect();
    SDL2pp::Rect get_weapon_rect(const std::string& key);
    SDL2pp::Rect get_bullet_rect(const std::string& key);
    std::shared_ptr<SDL2pp::Music> getMusicTrack(const std::string& key);
    std::shared_ptr<SDL2pp::Font> getFont(const std::string& key);
    std::shared_ptr<SDL2pp::Chunk> getSFX(const std::string& key);
    std::shared_ptr<SDL2pp::Texture> getTexture(const std::string& key);
    SDL2pp::Rect getAnimationFrame(const std::string& key, const int frame);
    void releaseResources();
};

#endif
