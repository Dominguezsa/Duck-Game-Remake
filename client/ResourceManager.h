#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Font.hh>
#include <SDL2pp/Music.hh>
#include <SDL2pp/Texture.hh>

class ResourceManager {
private:
    std::map<std::string, std::shared_ptr<SDL2pp::Chunk>> sfx;
    std::map<std::string, std::shared_ptr<SDL2pp::Music>> music;
    std::map<std::string, std::shared_ptr<SDL2pp::Font>> fonts;
    std::map<std::string, std::shared_ptr<SDL2pp::Texture>> textures;
    // This could go in another class really, like one that store graphics related stuff, another
    // that stores audio related stuff and so on
    std::map<std::string, std::vector<SDL2pp::Rect>> animationFrames;
    SDL2pp::Renderer& renderer;

public:
    explicit ResourceManager(SDL2pp::Renderer& renderer);
    ~ResourceManager();
    void loadResources();
    void loadSFX();
    void loadMusic();
    void loadFonts();
    void loadSprites();
    void loadAnimationFrames();
    std::shared_ptr<SDL2pp::Music> getMusicTrack(const std::string& key);
    std::shared_ptr<SDL2pp::Font> getFont(const std::string& key);
    std::shared_ptr<SDL2pp::Chunk> getSFX(const std::string& key);
    std::shared_ptr<SDL2pp::Texture> getTexture(const std::string& key);
    SDL2pp::Rect getAnimationFrame(const std::string& key, int frame);
    void releaseResources();
};

#endif
