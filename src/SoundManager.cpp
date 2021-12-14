#include "SoundManager.hpp"

SoundManager::SoundManager() {
    this->uneven_match = false;
    load();
}

SoundManager::~SoundManager() {
    Mix_FreeChunk(sound_pick);
    Mix_FreeChunk(sound_drop);
    Mix_FreeChunk(sound_match1);
    Mix_FreeChunk(sound_match2);
    Mix_FreeChunk(sound_pain);
}

void SoundManager::load() {
    sound_pick = Mix_LoadWAV("assets/sounds/pick.wav");
    sound_drop = Mix_LoadWAV("assets/sounds/drop.wav");
    sound_match1 = Mix_LoadWAV("assets/sounds/match1.wav");
    sound_match2 = Mix_LoadWAV("assets/sounds/match2.wav");
    sound_pain = Mix_LoadWAV("assets/sounds/pain.wav");
    sound_completed = Mix_LoadWAV("assets/sounds/completed.wav");
    sound_failed = Mix_LoadWAV("assets/sounds/failed.wav");

    if (sound_drop == NULL ||
        sound_drop == NULL ||
        sound_match1 == NULL ||
        sound_match2 == NULL ||
        sound_pain == NULL ||
        sound_completed == NULL ||
        sound_failed == NULL) {
            SDL_Log("Couldn't load all sounds: %s", Mix_GetError());
    }
}

void SoundManager::play(Sound sound) {
    switch (sound)
    {
    case Sound::PICK:
        Mix_PlayChannel(channel_pick, sound_pick, SDL_FALSE);
        break;
    case Sound::DROP:
        Mix_PlayChannel(channel_pick, sound_drop, SDL_FALSE);
        break;
    case Sound::MATCH:
        if (uneven_match) {
            Mix_PlayChannel(channel_match2, sound_match2, SDL_FALSE);
        } else {
            Mix_PlayChannel(channel_match1, sound_match1, SDL_FALSE);
        }
        uneven_match = !(uneven_match);
        break;
    case Sound::PAIN:
        Mix_HaltChannel(channel_match1);
        Mix_HaltChannel(channel_match2);
        Mix_PlayChannel(channel_notify, sound_pain, SDL_FALSE);
        break;
    case Sound::COMPLETED:
        Mix_HaltChannel(channel_match1);
        Mix_HaltChannel(channel_match2);
        Mix_PlayChannel(channel_notify, sound_completed, SDL_FALSE);
        break;
    case Sound::FAILED:
        Mix_HaltChannel(channel_match1);
        Mix_HaltChannel(channel_match2);
        Mix_PlayChannel(channel_notify, sound_failed, SDL_FALSE);
        break;
    }
}
