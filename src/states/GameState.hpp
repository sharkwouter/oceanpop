#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include <SDL.h>

#include <vector>
#include <filesystem>
#include <regex>

#include <json/json.h>

#include "BaseState.hpp"
#include "../BoardManager.hpp"
#include "../ThemeManager.hpp"
#include "../SoundManager.hpp"
#include "../FontManager.hpp"
#include "../OptionManager.hpp"
#include "../PauseScreen.hpp"

#include "../Shell.hpp"

class GameState : public BaseState {
private:
    SDL_Renderer * renderer;
    SoundManager * sounds;
    FontManager * fonts;
    OptionManager * options;

    BoardManager * board = NULL;
    ThemeManager theme;

    std::regex level_regex{"^.*[\\\\\\/]level[0-9][0-9][0-9]\\.json$"};
    int total_levels = 0;

    PauseScreen pause_screen;
    PauseScreen win_screen;
    PauseScreen lose_screen;

    bool paused = false;
    bool completed = false;
    bool failed = false;
    bool finished = false;

    int level;
    int seed;
    int moves;
    int required_matches;
    int width;
    int height;
    int current_theme;
    std::vector<std::vector<ShellType>> shells;

    State next_state = State::MENU;

    int getTotalLevels();
    void loadLevel(int level);
    std::vector<std::vector<ShellType>> loadShells(Json::Value array);
public:
    GameState(SDL_Renderer *renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options);
    ~GameState();

    void handleEvents(std::vector<Event> events);
    void update();
    void draw(SDL_Renderer *renderer);

    State getNextState();
};

#endif // GAMESTATE_HPP
