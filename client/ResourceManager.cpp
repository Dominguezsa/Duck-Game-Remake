#include "ResourceManager.h"

#include <memory>
#include <stdexcept>
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
#define DUCK_ARMS_WIDTH 16
#define DUCK_ARMS_HEIGHT 16

#define DUCK_JUMPING_ARMS_X 1
#define DUCK_JUMPING_ARMS_Y 534

#define DUCK_DUCKING_X 129
#define DUCK_DUCKING_Y 71

#define DUCK_SLIDING_X 1
#define DUCK_SLIDING_Y 71

#define DUCK_SLIDING_AIR_X 161
#define DUCK_SLIDING_AIR_Y 39

ResourceManager::ResourceManager(SDL2pp::Renderer& renderer): renderer(renderer) {
    // if (chdir("client") != 0) {
    //     throw std::runtime_error("Error: Could not change to 'client' directory\n");
    // }
}

void ResourceManager::loadSFX() {

    sfx.emplace("boom1",
                std::make_shared<SDL2pp::Chunk>("/var/duck_game/data/audio/sfx/boom1.wav"));
    sfx.emplace("boom2",
                std::make_shared<SDL2pp::Chunk>("/var/duck_game/data/audio/sfx/boom2.wav"));
    sfx.emplace("boom3",
                std::make_shared<SDL2pp::Chunk>("/var/duck_game/data/audio/sfx/boom3.wav"));
    sfx.emplace("boom11",
                std::make_shared<SDL2pp::Chunk>("/var/duck_game/data/audio/sfx/boom11.wav"));
    std::cout << "Every sfx loaded correctly\n";
}

void ResourceManager::loadMusic() {
    music.emplace("back_music",
                  std::make_shared<SDL2pp::Music>(
                          "/var/duck_game/data/audio/ost/back_music_space_mystery_out.ogg"));
}

void ResourceManager::loadFonts() {
    fonts.emplace("vera", std::make_shared<SDL2pp::Font>("/var/duck_game/data/Vera.ttf", 12));
}

void ResourceManager::loadSprites(uint8_t playerAmount) {

    textures.emplace("duck",
                     std::make_shared<SDL2pp::Texture>(
                             renderer, SDL2pp::Surface("/var/duck_game/data/imagenesDePatos.png")
                                               .SetColorKey(true, 0)));

    textures["duck"]->SetBlendMode(SDL_BLENDMODE_BLEND);

    textures.emplace("background",
                     std::make_shared<SDL2pp::Texture>(
                             renderer, SDL2pp::Surface("/var/duck_game/data/fondo.png")));

    textures.emplace("tablas",
                     std::make_shared<SDL2pp::Texture>(
                             renderer, SDL2pp::Surface("/var/duck_game/data/tablon1.png")));

    std::vector<std::string> duck_colors = {"white_duck", "orange_duck", "grey_duck",
                                            "yellow_duck"};

    std::cout << "Now loading " << +playerAmount << " ducks\n";

    for (int i = 0; i < playerAmount; i++) {

        std::string path = "/var/duck_game/data/sprites/ducks/" + duck_colors[i] + ".png";

        textures.emplace(duck_colors[i],
                         std::make_shared<SDL2pp::Texture>(renderer, SDL2pp::Surface(path)));
    }

    textures.emplace(
            "next_round",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("/var/duck_game/data/next_round.png")));

    textures.emplace(
            "ak47",
            std::make_shared<SDL2pp::Texture>(
                    renderer,
                    SDL2pp::Surface("/var/duck_game/data/sprites/weapons/machineguns.png")));

    textures.emplace(
            "grenade",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("/var/duck_game/data/sprites/weapons/grenades.png")));

    textures.emplace(
            "banana",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("/var/duck_game/data/sprites/weapons/grenades.png")));

    textures.emplace(
            "pewPewLaser",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("/var/duck_game/data/sprites/weapons/laser.png")));

    textures.emplace(
            "laserRifle",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("/var/duck_game/data/sprites/weapons/laser.png")));

    textures.emplace(
            "shotgun",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("/var/duck_game/data/sprites/weapons/shotguns.png")));

    textures.emplace(
            "sniper",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("/var/duck_game/data/sprites/weapons/snipers.png")));

    textures.emplace(
            "duelingPistol",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("/var/duck_game/data/sprites/weapons/pistols.png")));

    textures.emplace(
            "magnum",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("/var/duck_game/data/sprites/weapons/pistols.png")));

    textures.emplace(
            "cowboyPistol",
            std::make_shared<SDL2pp::Texture>(
                    renderer, SDL2pp::Surface("/var/duck_game/data/sprites/weapons/pistols.png")));

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
        duckFrames.emplace_back(SDL2pp::Rect(DUCK_RUNNING_ARMS_X + i * DUCK_ARMS_WIDTH,
                                             DUCK_RUNNING_ARMS_Y, DUCK_ARMS_WIDTH,
                                             DUCK_ARMS_HEIGHT));
    }

    animationFrames.emplace("duck_running_arms", duckFrames);

    duckFrames.clear();

    for (int i = 0; i < DUCK_JUMPING_FRAMES; i++) {
        duckFrames.emplace_back(SDL2pp::Rect(DUCK_JUMPING_ANIM_X + i * DUCK_RECT_WIDTH,
                                             DUCK_JUMPING_ANIM_Y, DUCK_RECT_WIDTH,
                                             DUCK_RECT_HEIGHT));
    }

    animationFrames.emplace("duck_jumping", duckFrames);

    duckFrames.clear();

    // This is for the running arms animation
    for (int i = 0; i < DUCK_RUNNING_ARMS_FRAMES; i++) {
        duckFrames.emplace_back(SDL2pp::Rect(DUCK_JUMPING_ARMS_X + i * DUCK_ARMS_WIDTH,
                                             DUCK_JUMPING_ARMS_Y, DUCK_ARMS_WIDTH,
                                             DUCK_ARMS_HEIGHT));
    }

    animationFrames.emplace("duck_jumping_arms", duckFrames);

    duckFrames.clear();

    duckFrames.emplace_back(
            SDL2pp::Rect(DUCK_DUCKING_X, DUCK_DUCKING_Y, DUCK_RECT_WIDTH, DUCK_RECT_HEIGHT));

    animationFrames.emplace("duck_ducking", duckFrames);

    duckFrames.clear();

    duckFrames.emplace_back(
            SDL2pp::Rect(DUCK_SLIDING_X, DUCK_SLIDING_Y, DUCK_RECT_WIDTH, DUCK_RECT_HEIGHT));

    animationFrames.emplace("duck_sliding", duckFrames);

    duckFrames.clear();

    duckFrames.emplace_back(SDL2pp::Rect(DUCK_SLIDING_AIR_X, DUCK_SLIDING_AIR_Y, DUCK_RECT_WIDTH,
                                         DUCK_RECT_HEIGHT));

    animationFrames.emplace("duck_sliding_air", duckFrames);

    duckFrames.clear();

    duckFrames.emplace_back(SDL2pp::Rect(0, 0, 0, 0));

    animationFrames.emplace("empty", duckFrames);
    // duckFrames.clear();

    // // This is for the guns
    // for (int i = 0; i < 1; i++) {
    //     duckFrames.emplace_back(SDL2pp::Rect(1, 14, DUCK_ARMS_WIDTH, DUCK_ARMS_HEIGHT));
    // }

    // animationFrames.emplace("ak47", duckFrames);

    // duckFrames.clear();

    // duckFrames.emplace_back(SDL2pp::Rect(2, 205, 8, 8));
    // animationFrames.emplace("ak47_bullet", duckFrames);


    std::cout << "All animation frames loaded correctly\n";
}


void ResourceManager::load_weapons_rect() {
    SDL2pp::Rect weaponFrame;
    weaponFrame = SDL2pp::Rect(1, 19, 32, 32);
    weaponFrames.emplace("ak47", weaponFrame);

    weaponFrame = SDL2pp::Rect(1, 19, 32, 32);
    weaponFrames.emplace("grenade", weaponFrame);

    weaponFrame = SDL2pp::Rect(1, 19, 32, 32);
    weaponFrames.emplace("banana", weaponFrame);

    weaponFrame = SDL2pp::Rect(1, 19, 32, 32);
    weaponFrames.emplace("pewPewLaser", weaponFrame);

    weaponFrame = SDL2pp::Rect(1, 19, 32, 32);
    weaponFrames.emplace("laserRifle", weaponFrame);

    weaponFrame = SDL2pp::Rect(1, 19, 32, 32);
    weaponFrames.emplace("shotgun", weaponFrame);

    weaponFrame = SDL2pp::Rect(1, 19, 32, 32);
    weaponFrames.emplace("sniper", weaponFrame);

    weaponFrame = SDL2pp::Rect(1, 19, 32, 32);
    weaponFrames.emplace("duelingPistol", weaponFrame);

    weaponFrame = SDL2pp::Rect(1, 19, 32, 32);
    weaponFrames.emplace("magnum", weaponFrame);

    weaponFrame = SDL2pp::Rect(1, 19, 32, 32);
    weaponFrames.emplace("cowboyPistol", weaponFrame);

    std::cout << "All weapons rects loaded correctly\n";
}

void ResourceManager::load_bullets_rect() {
    SDL2pp::Rect bulletFrame;
    bulletFrame = SDL2pp::Rect(2, 205, 8, 8);
    bulletFrames.emplace("ak47", bulletFrame);
}


SDL2pp::Rect ResourceManager::get_weapon_rect(const std::string& key) { return weaponFrames[key]; }

SDL2pp::Rect ResourceManager::get_bullet_rect(const std::string& key) { return bulletFrames[key]; }

void ResourceManager::loadResources(uint8_t playerAmount) {
    std::cout << "Trying to load the sfx\n";
    loadSFX();
    std::cout << "Trying to load the music\n";
    loadMusic();
    loadFonts();
    loadSprites(playerAmount);
    load_weapons_rect();
    load_bullets_rect();
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
