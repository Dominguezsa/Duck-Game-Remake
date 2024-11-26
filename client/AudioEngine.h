#ifndef AUDIO_ENGINE_H
#define AUDIO_ENGINE_H

#include <vector>

#include <SDL2pp/Mixer.hh>

#include "../common/types/duck_state.h"

#include "ResourceManager.h"

class AudioEngine {
private:
    std::vector<DuckState>& ducks;
    SDL2pp::Mixer& mixer;
    ResourceManager& resourceManager;

public:
    AudioEngine(std::vector<DuckState>& ducks, SDL2pp::Mixer& mixer,
                ResourceManager& resourceManager);

    void playAudio();

    ~AudioEngine() = default;
};


#endif  // AUDIO_ENGINE_H
