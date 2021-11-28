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

    bool uneven_match;
    bool pain_playing;

    int channel_pick = 1;
    int channel_match1 = 2;
    int channel_match2 = 3;
    int channel_pain = 3;
public:
    SoundManager();
    ~SoundManager();

    void load();
    void play(Sound sound);
};

#endif // SOUNDMANAGER_HPP