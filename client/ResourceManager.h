#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <SDL2pp/Chunk.hh>
#include <SDL2pp/Font.hh>
#include <SDL2pp/Music.hh>
#include <SDL2pp/Texture.hh>

class ResourceManager {
private:
    // cppcheck-suppress unusedStructMember
    std::map<std::string, SDL2pp::Chunk> sfx;
    // cppcheck-suppress unusedStructMember
    std::map<std::string, SDL2pp::Music> music;
    // cppcheck-suppress unusedStructMember
    std::map<std::string, SDL2pp::Font> fonts;
    // cppcheck-suppress unusedStructMember
    std::map<std::string, SDL2pp::Texture> textures;
    SDL2pp::Renderer& renderer;


public:
    explicit ResourceManager(SDL2pp::Renderer& renderer);
    ~ResourceManager();
    void loadResources();
    void loadSFX();
    void loadMusic();
    void loadFonts();
    void loadSprites();
    void releaseResources();
};

#endif
