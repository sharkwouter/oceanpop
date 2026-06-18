#ifndef SOUNDMANAGER_HPP
#define SOUNDMANAGER_HPP

#include <string>
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>

#include "OptionManager.hpp"
#include "Sound.hpp"

class SoundManager {
private:
    OptionManager * options;

    MIX_Audio * sound_pick;
    MIX_Audio * sound_drop;
    MIX_Audio * sound_match1;
    MIX_Audio * sound_match2;
    MIX_Audio * sound_pain;
    MIX_Audio * sound_completed;
    MIX_Audio * sound_failed;

    MIX_Mixer * mixer = nullptr;

    MIX_Track * channel_pick = nullptr;
    MIX_Track * channel_match1 = nullptr;
    MIX_Track * channel_match2 = nullptr;
    MIX_Track * channel_notify = nullptr;

    bool uneven_match;

    void load();
public:
    SoundManager(OptionManager * options);
    ~SoundManager();

    void play(Sound sound);

    MIX_Mixer * getMixer() {return this->mixer;};
};

#endif // SOUNDMANAGER_HPP