#include "SoundManager.hpp"

#include "utils.hpp"

SoundManager::SoundManager(OptionManager * options) : options(options) {
    this->uneven_match = false;
    load();
}

SoundManager::~SoundManager() {
    #ifndef NXDK
    Mix_FreeChunk(sound_pick);
    Mix_FreeChunk(sound_drop);
    Mix_FreeChunk(sound_match1);
    Mix_FreeChunk(sound_match2);
    Mix_FreeChunk(sound_pain);
    Mix_FreeChunk(sound_completed);
    Mix_FreeChunk(sound_failed);
    #endif
}

void SoundManager::load() {
    #ifndef NXDK
    sound_pick = Mix_LoadWAV(getResourcePath("assets/sounds/pick.wav").c_str());
    sound_drop = Mix_LoadWAV(getResourcePath("assets/sounds/drop.wav").c_str());
    sound_match1 = Mix_LoadWAV(getResourcePath("assets/sounds/match1.wav").c_str());
    sound_match2 = Mix_LoadWAV(getResourcePath("assets/sounds/match2.wav").c_str());
    sound_pain = Mix_LoadWAV(getResourcePath("assets/sounds/pain.wav").c_str());
    sound_completed = Mix_LoadWAV(getResourcePath("assets/sounds/completed.wav").c_str());
    sound_failed = Mix_LoadWAV(getResourcePath("assets/sounds/failed.wav").c_str());

    if (sound_drop == NULL ||
        sound_drop == NULL ||
        sound_match1 == NULL ||
        sound_match2 == NULL ||
        sound_pain == NULL ||
        sound_completed == NULL ||
        sound_failed == NULL) {
            SDL_Log("Couldn't load all sounds: %s", Mix_GetError());
    }
    #endif
}

void SoundManager::play(Sound sound) {
    #ifndef NXDK
    int volume = MIX_MAX_VOLUME/4*this->options->getSoundVolume();
    switch (sound) {
        case Sound::PICK:
            Mix_VolumeChunk(sound_pick, volume);
            Mix_PlayChannel(channel_pick, sound_pick, SDL_FALSE);
            break;
        case Sound::DROP:
            Mix_VolumeChunk(sound_drop, volume);
            Mix_PlayChannel(channel_pick, sound_drop, SDL_FALSE);
            break;
        case Sound::MATCH:
            if (uneven_match) {
                Mix_VolumeChunk(sound_match2, volume);
                Mix_PlayChannel(channel_match2, sound_match2, SDL_FALSE);
            } else {
                Mix_VolumeChunk(sound_match1, volume);
                Mix_PlayChannel(channel_match1, sound_match1, SDL_FALSE);
            }
            uneven_match = !(uneven_match);
            break;
        case Sound::PAIN:
            Mix_VolumeChunk(sound_pain, volume);
            Mix_HaltChannel(channel_match1);
            Mix_HaltChannel(channel_match2);
            Mix_PlayChannel(channel_notify, sound_pain, SDL_FALSE);
            break;
        case Sound::COMPLETED:
            Mix_VolumeChunk(sound_completed, volume);
            Mix_HaltChannel(channel_match1);
            Mix_HaltChannel(channel_match2);
            Mix_PlayChannel(channel_notify, sound_completed, SDL_FALSE);
            break;
        case Sound::FAILED:
            Mix_VolumeChunk(sound_failed, volume);
            Mix_HaltChannel(channel_match1);
            Mix_HaltChannel(channel_match2);
            Mix_PlayChannel(channel_notify, sound_failed, SDL_FALSE);
            break;
    }
    #endif
}
