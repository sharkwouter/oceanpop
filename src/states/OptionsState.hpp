#ifndef OPTIONSSTATE_HPP
#define OPTIONSSTATE_HPP

#include <string>
#include <vector>
#include <SDL.h>
#include "BaseState.hpp"
#include "../ThemeManager.hpp"
#include "../FontManager.hpp"
#include "../SoundManager.hpp"
#include "../OptionManager.hpp"
#include "../PauseScreen.hpp"

enum class Option {
    CHANGE_MUSIC,
    SOUND_VOLUME,
    MUSIC_VOLUME,
    FULLSCREEN,
    RESOLUTION,
    GO_BACK
};

class OptionsState : public BaseState {
private:
    SDL_Renderer * renderer;
    FontManager * fonts;
    SoundManager * sounds;
    OptionManager * options;

    ThemeManager theme;

    SDL_Texture * text_title;
    std::vector<SDL_Texture*> texts;

    int text_offset = 2;
    int text_start_y;

    int selection = 0;

    int getTextY(int number);
    std::string getChangeMusicString();
    std::string getSoundVolumeString();
    std::string getMusicVolumeString();
    std::string getFullscreenString();
    std::string getResolutionString();

    void changeChangeMusic();
    void changeSoundVolume(int amount);
    void changeMusicVolume(int amount);
    void changeFullscreen();
    void changeResolution(int amount);
public:
    OptionsState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~OptionsState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer * renderer);

    State getNextState();
};

#endif // OPTIONSSTATE_HPP
