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

    this->sounds_track = MIX_CreateTrack(this->mixer);
    this->sound_pick = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/pick.wav").c_str(), false);
    this->sound_drop = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/drop.wav").c_str(), false);
    this->sound_match1 = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/match1.wav").c_str(), false);
    this->sound_match2 = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/match2.wav").c_str(), false);
    this->sound_pain = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/pain.wav").c_str(), false);
    this->sound_completed = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/completed.wav").c_str(), false);
    this->sound_failed = MIX_LoadAudio(this->mixer, getResourcePath("assets/sounds/failed.wav").c_str(), false);

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
            MIX_SetTrackGain(this->sounds_track, volume);
            MIX_SetTrackAudio(this->sounds_track, sound_pick);
            MIX_PlayTrack(this->sounds_track, 0);
            break;
        case Sound::DROP:
            MIX_SetTrackGain(this->sounds_track, volume);
            MIX_SetTrackAudio(this->sounds_track, sound_drop);
            MIX_PlayTrack(this->sounds_track, 0);
            break;
        case Sound::MATCH:
            if (uneven_match) {
                MIX_SetTrackGain(this->sounds_track, volume);
                MIX_SetTrackAudio(this->sounds_track, sound_match2);
                MIX_PlayTrack(this->sounds_track, 0);
            } else {
                MIX_SetTrackGain(this->sounds_track, volume);
                MIX_SetTrackAudio(this->sounds_track, sound_match1);
                MIX_PlayTrack(this->sounds_track, 0);
            }
            uneven_match = !(uneven_match);
            break;
        case Sound::PAIN:
            MIX_SetTrackGain(this->sounds_track, volume);
            MIX_StopTrack(this->sounds_track, 0);
            MIX_SetTrackAudio(this->sounds_track, sound_pain);
            MIX_PlayTrack(this->sounds_track, 0);
            break;
        case Sound::COMPLETED:
            MIX_SetTrackGain(this->sounds_track, volume);
            MIX_StopTrack(this->sounds_track, 0);
            MIX_SetTrackAudio(this->sounds_track, sound_completed);
            MIX_PlayTrack(this->sounds_track, 0);
            break;
        case Sound::FAILED:
            MIX_SetTrackGain(this->sounds_track, volume);
            MIX_StopTrack(this->sounds_track, 0);
            MIX_SetTrackAudio(this->sounds_track, sound_failed);
            MIX_PlayTrack(this->sounds_track, 0);
            break;
    }
}
