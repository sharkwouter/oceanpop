#include "GameState.hpp"

#include <SDL_ttf.h>
#include <json/json.h>

#include <cmath>
#include <filesystem>
#include <fstream>

#include "../colors.hpp"

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
    for (auto &entry : level_files) {
        levels.push_back(entry.path());
    }
    this->level = 0;

    loadLevel();
}

GameState::~GameState() {
    delete(this->board);
}

void GameState::update() {
    if (this->board->isCompleted() && !this->completed) {
        theme.pause();
        sounds->play(Sound::COMPLETED);
        this->completed = true;
    } else if (!this->board->hasMovesLeft() && !this->failed && !this->completed) {
        theme.pause();
        sounds->play(Sound::FAILED);
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
                theme.pause();
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
    if (this->level >= (int) this->levels.size() || this->level < 0) {
        this->level = 0;
    }

    Json::Value json;

    std::ifstream levelStream;
    levelStream.open(this->levels[this->level], std::ios::binary);
    Json::CharReaderBuilder builder;
    JSONCPP_STRING errors;
    Json::parseFromStream(builder, levelStream, &json, &errors);
    levelStream.close();

    this->width = json.get("width", BOARD_WIDTH).asInt();
    this->height = json.get("height", BOARD_HEIGHT).asInt();

    this->position = calculatePosition(this->width, this->height);

    this->moves = json.get("moves", 10).asInt();
    this->required_matches = json.get("matches", 12).asInt();
    this->seed = json.get("seed", this->level).asInt();

    if (this->board != NULL) {
        delete this->board;
    }
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
        this->level + 1,
        this->seed
    );

    theme.switchTheme(json.get("theme", 1).asInt());
    if (this->moves < 3) {
        theme.pause();
    }
}

SDL_Point GameState::calculatePosition(int width, int height) {
    return {(SCREEN_WIDTH-SHELL_SIZE*width)/2, (SCREEN_HEIGHT-SHELL_SIZE*(height+1))/2};
}

State GameState::getNextState() {
    return State::MENU;
}
