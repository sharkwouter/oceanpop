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
#include "../constants.hpp"

enum class Option {
    CHANGE_MUSIC,
    SOUND_VOLUME,
    MUSIC_VOLUME,
#ifdef SHOW_RESOLUTION_OPTIONS
    FULLSCREEN,
    RESOLUTION,
#endif
    APPLY,
    GO_BACK
};

class OptionsState : public BaseState {
private:
    SDL_Renderer * renderer;
    FontManager * fonts;
    SoundManager * sounds;
    OptionManager * options;
    SDL_Window * window;

    ThemeManager theme;

    SDL_Texture * text_title;
    std::vector<SDL_Texture*> texts;

    State next_state = State::MENU;

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

    void applyChanges();
    void applyFullscreen();
    void applyResolution();

    bool change_music;
    int sound_volume;
    int music_volume;
    bool fullscreen;
    int screen_width;
    int screen_height;
    int screen_refresh_rate;

    void updateTexts();
    void loadTexts();
public:
    OptionsState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options, SDL_Window * window);
    ~OptionsState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer * renderer);

    State getNextState();
};

#endif // OPTIONSSTATE_HPP
