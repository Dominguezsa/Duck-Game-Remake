#include "AudioEngine.h"

AudioEngine::AudioEngine(std::vector<DuckState>& ducks, SDL2pp::Mixer& mixer,
                         ResourceManager& resourceManager):
        ducks(ducks), mixer(mixer), resourceManager(resourceManager) {}

void AudioEngine::playAudio() {
    for (const auto& duck: ducks) {
        if (duck.is_shooting) {
            mixer.PlayChannel(1, *resourceManager.getSFXForWeapon(duck.weapon), 0);
        }
    }
}
