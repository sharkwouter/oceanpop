#include "SoundManager.hpp"

#include "utils.hpp"

SoundManager::SoundManager(OptionManager * options) : options(options) {
    this->uneven_match = false;
    load();
}

SoundManager::~SoundManager() {
    MIX_DestroyAudio(sound_pick);
    MIX_DestroyAudio(sound_drop);
    MIX_DestroyAudio(sound_match1);
    MIX_DestroyAudio(sound_match2);
    MIX_DestroyAudio(sound_pain);
    MIX_DestroyAudio(sound_completed);
    MIX_DestroyAudio(sound_failed);
    MIX_DestroyMixer(this->mixer);
}

void SoundManager::load() {
    this->mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (!this->mixer) {
      panic("Couldn't create mixer on default device: " + std::string(SDL_GetError()));
    }

    this->channel_pick = MIX_CreateTrack(this->mixer);;
    this->channel_match1 = MIX_CreateTrack(this->mixer);
    this->channel_match2 = MIX_CreateTrack(this->mixer);
    this->channel_notify = MIX_CreateTrack(this->mixer);

    this->sound_pick = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/pick.wav").c_str(), true);
    this->sound_drop = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/drop.wav").c_str(), true);
    this->sound_match1 = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/match1.wav").c_str(), true);
    this->sound_match2 = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/match2.wav").c_str(), true);
    this->sound_pain = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/pain.wav").c_str(), true);
    this->sound_completed = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/completed.wav").c_str(), true);
    this->sound_failed = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/failed.wav").c_str(), true);

    if (this->sound_drop == NULL ||
        this->sound_drop == NULL ||
        this->sound_match1 == NULL ||
        this->sound_match2 == NULL ||
        this->sound_pain == NULL ||
        this->sound_completed == NULL ||
        this->sound_failed == NULL) {
            SDL_Log("Couldn't load all sounds: %s", SDL_GetError());
    }
}

void SoundManager::play(Sound sound) {
    float volume = 0.25f * (float) this->options->getSoundVolume();
    switch (sound) {
        case Sound::PICK:
            MIX_StopTrack(this->channel_pick, 0);
            MIX_SetTrackGain(this->channel_pick, volume);
            MIX_SetTrackAudio(this->channel_pick, sound_pick);
            MIX_PlayTrack(this->channel_pick, 0);
            break;
        case Sound::DROP:
            MIX_StopTrack(this->channel_pick, 0);
            MIX_SetTrackGain(this->channel_pick, volume);
            MIX_SetTrackAudio(this->channel_pick, sound_drop);
            MIX_PlayTrack(this->channel_pick, 0);
            break;
        case Sound::MATCH:
            if (uneven_match) {
                MIX_StopTrack(this->channel_match1, 0);
                MIX_SetTrackGain(this->channel_match1, volume);
                MIX_SetTrackAudio(this->channel_match1, sound_match2);
                MIX_PlayTrack(this->channel_match1, 0);
            } else {
                MIX_StopTrack(this->channel_match2, 0);
                MIX_SetTrackGain(this->channel_match2, volume);
                MIX_SetTrackAudio(this->channel_match2, sound_match1);
                MIX_PlayTrack(this->channel_match2, 0);
            }
            uneven_match = !(uneven_match);
            break;
        case Sound::PAIN:
            MIX_SetTrackGain(this->channel_notify, volume);
            MIX_StopTrack(this->channel_match1, 0);
            MIX_StopTrack(this->channel_match2, 0);
            MIX_StopTrack(this->channel_notify, 0);
            MIX_SetTrackAudio(this->channel_notify, sound_pain);
            MIX_PlayTrack(this->channel_notify, 0);
            break;
        case Sound::COMPLETED:
            MIX_SetTrackGain(this->channel_notify, volume);
            MIX_StopTrack(this->channel_match1, 0);
            MIX_StopTrack(this->channel_match2, 0);
            MIX_StopTrack(this->channel_notify, 0);
            MIX_SetTrackAudio(this->channel_notify, sound_completed);
            MIX_PlayTrack(this->channel_notify, 0);
            break;
        case Sound::FAILED:
            MIX_SetTrackGain(this->channel_notify, volume);
            MIX_StopTrack(this->channel_match1, 0);
            MIX_StopTrack(this->channel_match2, 0);
            MIX_StopTrack(this->channel_notify, 0);
            MIX_SetTrackAudio(this->channel_notify, sound_failed);
            MIX_PlayTrack(this->channel_notify, 0);
            break;
    }
}
