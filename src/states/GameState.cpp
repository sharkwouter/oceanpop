#include "GameState.hpp"

#include <SDL_ttf.h>

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "../colors.hpp"
#include "../utils.hpp"

GameState::GameState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds) :
    theme(renderer, Theme::THEME1),
    pause_screen(renderer, "Game Paused", "Press the confirm button to exit"),
    win_screen(renderer, "Level Finished!", "Press the confirm button to continue"),
    lose_screen(renderer, "Level Failed", "Press the confirm button to restart"),
    finish_screen(renderer, "Game Completed!", "Press the confirm button to exit")
{
    this->renderer = renderer;
    this->fonts = fonts;
    this->sounds = sounds;

    std::filesystem::directory_iterator level_files("assets/levels/");
    this->total_levels = 0;
    int highest_level = 0;
    for (auto &entry : level_files) {
        if (std::regex_match(entry.path().string(), level_regex)) {
            std::string name = entry.path().stem().string();
            int number = std::stoi(name.substr(5));

            SDL_Log("Found level: %d", number);
            if (number > highest_level) {
                highest_level = number;
            }
            this->total_levels++;
        }
    }
    if (this->total_levels != highest_level) {
        panic("Found " + std::to_string(this->total_levels) + "levels, the highest being " + std::to_string(highest_level) + ". This does not add up. Quitting!");
    }

    this->level = 1;
    loadLevel();
}

GameState::~GameState() {
    delete(this->board);
}

void GameState::update() {
    if (this->board->isCompleted() && !this->completed) {
        this->theme.pause();
        this->sounds->play(Sound::COMPLETED);
        this->completed = true;
    } else if (!this->board->hasMovesLeft() && !this->failed && !this->completed) {
        this->theme.pause();
        this->sounds->play(Sound::FAILED);
        this->failed = true;
    }

    if (!this->paused && !this->completed &&  !this->failed) {
        this->theme.update();
        this->board->update();
    }
}

void GameState::handleEvents(std::vector<Event> events) {
    if (!this->paused && !this->completed && !this->failed) {
        this->board->handleEvents(events);
    }

    for(Event event: events) {
         if (this->completed) {
            if (event == Event::CONFIRM) {
                this->completed = false;
                this->theme.next();
                this->level++;
                this->loadLevel();
            }
        } else if (this->failed) {
            if (event == Event::CONFIRM) {
                this->failed = false;
                this->board->reset();
                this->theme.unpause();
            }
        } else if (this->paused) {
            if (event == Event::CONFIRM) {
                this->done = true;
            } else if (event == Event::CANCEL || event == Event::MENU) {
                this->paused = false;
            }
        } else if (event == Event::MENU) {
            this->paused = !(this->paused);
            return;
        }
    }
}

void GameState::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    if (this->completed) {
        win_screen.draw(renderer);
    } else if (this->failed) {
        lose_screen.draw(renderer);
    } else if (this->paused) {
        pause_screen.draw(renderer);
    } else {
        this->board->draw(renderer);
    }
}

void GameState::loadLevel() {
    if (this->level > this->total_levels || this->level <= 0) {
        this->level = 1;
    }

    std::ifstream levelStream;
    Json::CharReaderBuilder builder;
    Json::Value json;
    JSONCPP_STRING errors;
    const char * filename_base = "assets/levels/level%03d.json";
    char * filename = (char*) malloc(std::strlen(filename_base));

    std::sprintf(filename, filename_base, this->level);
    SDL_Log("Trying to load level: %s", filename);
    levelStream.open(filename, std::ios::binary);
    Json::parseFromStream(builder, levelStream, &json, &errors);
    levelStream.close();
    free(filename);

    if (errors.length() > 0) {
        panic("Failed to read level file with error: " + errors);
    }

    this->shells = loadShells(json["shells"]);
    if ((int) this->shells.size() == 0) {
        this->width = json.get("width", BOARD_WIDTH).asInt();
        this->height = json.get("height", BOARD_HEIGHT).asInt();
    } else {
        this->width = this->shells.size();
        this->height = this->shells[0].size();
    }

    this->position = calculatePosition(this->width, this->height);

    this->moves = json.get("moves", 10).asInt();
    this->required_matches = json.get("matches", 12).asInt();
    this->seed = json.get("seed", this->level).asInt();

    if (this->board != NULL) {
        delete this->board;
    }
    if ((int) this->shells.size() == 0) {
        this->board = new BoardManager(
            renderer,
            this->fonts,
            this->sounds,
            this->position.x,
            this->position.y,
            this->width,
            this->height,
            this->moves,
            this->required_matches,
            this->level,
            this->seed
        );
    } else {
        this->board = new BoardManager(
            renderer,
            this->fonts,
            this->sounds,
            this->position.x,
            this->position.y,
            this->shells,
            this->moves,
            this->required_matches,
            this->level,
            this->seed
        );
    }

    theme.switchTheme(json.get("theme", 1).asInt());
    if (this->moves < 3) {
        theme.pause();
    }
}

std::vector<std::vector<ShellType>> GameState::loadShells(Json::Value array) {
    std::vector<std::vector<ShellType>> result;

    if (array.empty() || !array.isArray() || (int) array.size() == 0) {
       return result;
    }

    result.reserve((int) array[0].size());
    for (int x = 0; x < (int) array[0].size(); x++) {
        result.push_back(std::move(std::vector<ShellType>()));
        result[x].reserve((int) array.size());
        for (int y = 0; y < (int) array.size(); y++) {
            int shell = array[y][x].asInt();
            if (shell < 0 || shell >= (int) ShellType::NUMBER_OF_COLORS) {
                SDL_Log("Not loading level, unknown shell number: %d", shell);
                return std::vector<std::vector<ShellType>>();
            }
            result[x].push_back((ShellType) shell);
        }
    }

    return result;
}

SDL_Point GameState::calculatePosition(int width, int height) {
    return {(SCREEN_WIDTH-SHELL_SIZE*width)/2, (SCREEN_HEIGHT-SHELL_SIZE*(height+1))/2};
}

State GameState::getNextState() {
    return State::MENU;
}
