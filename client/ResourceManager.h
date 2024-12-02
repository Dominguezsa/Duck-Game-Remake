#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Font.hh>
#include <SDL2pp/Music.hh>
#include <SDL2pp/Texture.hh>
#include <unistd.h>

#include "../common/types/weapon_type.h"

class ResourceManager {
private:
    std::map<std::string, std::shared_ptr<SDL2pp::Chunk>> sfx;
    std::map<WeaponType, std::shared_ptr<SDL2pp::Chunk>> sfxPerWeapon;
    std::map<std::string, std::shared_ptr<SDL2pp::Music>> music;
    std::map<std::string, std::shared_ptr<SDL2pp::Font>> fonts;
    std::map<std::string, std::shared_ptr<SDL2pp::Texture>> textures;
    std::map<std::string, std::vector<SDL2pp::Rect>> animationFrames;
    std::map<std::string, SDL2pp::Rect> weaponFrames;
    std::map<std::string, std::pair<SDL2pp::Rect, SDL2pp::Rect>> bulletFrames;

    std::map<std::string, SDL2pp::Rect> platformRect;
    SDL2pp::Renderer& renderer;

public:
    explicit ResourceManager(SDL2pp::Renderer& renderer);
    ~ResourceManager();
    void loadResources(uint8_t playerAmount);
    void loadSFX();
    void loadMusic();
    void loadFonts();
    void loadSprites(uint8_t playerAmount);
    void loadWeaponSprites();
    void loadDuckSprites(uint8_t playerAmount);
    void loadMiscSprites();
    void loadAnimationFrames();
    void loadWeaponsRect();
    void loadBulletsRect();
    void loadPlatformsRect();
    SDL2pp::Rect getWeaponRect(const std::string& key);
    std::pair<SDL2pp::Rect, SDL2pp::Rect> getBulletRect(const std::string& key);
    std::shared_ptr<SDL2pp::Music> getMusicTrack(const std::string& key);
    std::shared_ptr<SDL2pp::Font> getFont(const std::string& key);
    std::shared_ptr<SDL2pp::Chunk> getSFXForWeapon(const WeaponType& weapon);
    std::shared_ptr<SDL2pp::Texture> getTexture(const std::string& key);
    SDL2pp::Rect getPlatformRect(const std::string& key);
    SDL2pp::Rect getAnimationFrame(const std::string& key, const int frame);
    void releaseResources();
};

#endif
