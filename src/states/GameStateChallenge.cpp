#include "GameStateChallenge.hpp"

#include <SDL_ttf.h>
#include <cmath>
#include <string>
#include "../colors.hpp"

GameStateChallenge::GameStateChallenge(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) :
    theme(renderer, options, Theme::NONE),
    pause_screen(renderer, fonts, options, "Game Paused", "Press the confirm button to exit"),
    win_screen(renderer, fonts, options, "Level Finished!", "Press the confirm button to continue"),
    lose_screen(renderer, fonts, options, "Level failed", "Press the confirm button to restart")
{
    this->renderer = renderer;
    this->fonts = fonts;
    this->sounds = sounds;
    this->options = options;

    loadLevel();
}

GameStateChallenge::~GameStateChallenge() {
    SDL_DestroyTexture(text_attempts);
    SDL_DestroyTexture(text_attempts_number);
    delete(this->board);
}

void GameStateChallenge::update() {
    if (this->attempts == 0) {
        if(this->options->getChallengeModeHighscore() < this->level) {
            this->options->setChallengeModeHighscore(this->level);
        }
        this->options->resetChallengeMode();
        this->game_over = true;
        this->done = true;
    } else if (this->board->isCompleted() && !this->completed) {
        this->theme.pause();
        this->sounds->play(Sound::COMPLETED);
        this->completed = true;
    } else if (!this->board->hasMovesLeft() && !this->failed && !this->completed) {
        this->attempts--;
        this->attempts_changed = true;
        this->theme.pause();
        this->sounds->play(Sound::FAILED);
        this->failed = true;
    }

    if (!this->paused && !this->completed &&  !this->failed) {
        this->theme.update();
        this->board->update();
    }
}

void GameStateChallenge::handleEvents(std::vector<Event> events) {
    if (!this->paused && !this->completed &&  !this->failed) {
        this->board->handleEvents(events);
    }

    for(Event event: events) {
        if (this->completed) {
            if (event == Event::CONFIRM) {
                this->completed = false;
                this->theme.next();
                this->board->loadLevel(this->position.x, this->position.y, this->width, this->height, this->moves, ++this->required_matches, ++this->level, ++this->seed);
            }
        } else if (this->failed) {
            if (event == Event::CONFIRM) {
                this->failed = false;
                this->board->reset();
                this->theme.unpause();
            }
        } else if (this->paused) {
            if (event == Event::CONFIRM || event == Event::QUIT) {
                this->options->setChallengeModeLevel(this->level);
                this->options->setChallengeModeLives(this->attempts);
                this->done = true;
            } else if (event == Event::CANCEL || event == Event::MENU) {
                this->paused = false;
            }
        } else if (event == Event::MENU || event == Event::QUIT) {
            this->paused = true;
            return;
        }
    }
}

void GameStateChallenge::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    if (this->completed) {
        win_screen.draw(renderer);
    } else if (this->failed) {
        lose_screen.draw(renderer);
    } else if (this->paused) {
        pause_screen.draw(renderer);
    } else {
        drawAttempts(renderer);
        this->board->draw(renderer);
    }
}

void GameStateChallenge::drawAttempts(SDL_Renderer * renderer) {
    if(text_attempts == NULL) {
        text_attempts = fonts->getTexture(renderer, "lives left", false, {255, 255, 255, 255});
    }
    if (this->attempts_changed) {
        if (text_attempts_number != NULL) {
            SDL_DestroyTexture(text_attempts_number);
        }
        text_attempts_number = fonts->getTexture(renderer, std::to_string(this->attempts), false, {255, 255, 255, 255});
        this->attempts_changed = false;
    }
    // Render
    SDL_Rect rect_attempts = {this->position.x, this->position.y, 0, 0};
    SDL_QueryTexture(text_attempts, NULL, NULL, &rect_attempts.w, &rect_attempts.h);
    rect_attempts.x -= rect_attempts.w + this->options->getShellSize() / 2;
    SDL_RenderCopy(renderer, text_attempts, NULL, &rect_attempts);

    // Render number
    SDL_Rect rect_attempts_number = {rect_attempts.x + rect_attempts.w / 2, rect_attempts.y + rect_attempts.h, 0, 0};
    SDL_QueryTexture(text_attempts_number, NULL, NULL, &rect_attempts_number.w, &rect_attempts_number.h);
    rect_attempts_number.x -= rect_attempts_number.w / 2;
    SDL_RenderCopy(renderer, text_attempts_number, NULL, &rect_attempts_number);
}

void GameStateChallenge::loadLevel() {
    this->width = DEFAULT_BOARD_WIDTH;
    this->height = DEFAULT_BOARD_HEIGHT;

    this->position = calculatePosition(this->width, this->height);

    this->moves = 10;
    this->level = this->options->getChallengeModeLevel();
    this->required_matches = 11 + this->level;
    this->seed = this->level;

    

    this->board = new BoardManager(
        renderer,
        this->fonts,
        this->sounds,
        this->options,
        this->position.x,
        this->position.y,
        this->width,
        this->height,
        this->moves,
        this->required_matches,
        this->level,
        this->seed
    );

    this->theme.switchTheme(this->level - 1);
    if (this->level > 1) {
        this->attempts = this->options->getChallengeModeLives();
    } else {
        this->attempts = 3;
    }
    this->attempts_changed = true;
}

SDL_Point GameStateChallenge::calculatePosition(int width, int height) {
    return {(this->options->getScreenWidth()-this->options->getShellSize()*width)/2, (this->options->getScreenHeight()-this->options->getShellSize()*(height+1))/2};
}

State GameStateChallenge::getNextState() {
    if (this->game_over) {
        return State::GAMEOVER;
    } else {
        return State::MENU;
    }
}
