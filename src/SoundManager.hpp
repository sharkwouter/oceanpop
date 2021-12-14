#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <string>
#include <SDL.h>
#include <SDL_mixer.h>

#include "Sound.hpp"

class SoundManager {
private:
    Mix_Chunk * sound_pick;
    Mix_Chunk * sound_drop;
    Mix_Chunk * sound_match1;
    Mix_Chunk * sound_match2;
    Mix_Chunk * sound_pain;
    Mix_Chunk * sound_completed;
    Mix_Chunk * sound_failed;

    bool uneven_match;

    int channel_pick = 1;
    int channel_match1 = 2;
    int channel_match2 = 3;
    int channel_notify = 3;

    void load();
public:
    SoundManager();
    ~SoundManager();

    void play(Sound sound);
};

#endif // SOUNDMANAGER_HPP