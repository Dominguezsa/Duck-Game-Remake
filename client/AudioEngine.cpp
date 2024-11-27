#include "AudioEngine.h"

AudioEngine::AudioEngine(std::vector<DuckState>& ducks, SDL2pp::Mixer& mixer,
                         ResourceManager& resourceManager):
        ducks(ducks), mixer(mixer), resourceManager(resourceManager) {}

void AudioEngine::playAudio() {
    for (const auto& duck: ducks) {
        if (duck.is_shooting) {
            if (mixer.IsChannelPlaying(1) == 1) {
                continue;
            }
            mixer.PlayChannel(1, *resourceManager.getSFX("boom11"), -1);
        } else {
            mixer.FadeOutChannel(1, 200);
        }
    }
}
