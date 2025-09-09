#include "GameStateChallenge.hpp"

#include <SDL_ttf.h>
#include <cmath>
#include <string>
#include <cstdlib>
#include "../colors.hpp"
#include "../FontType.hpp"
#include "../utils.hpp"
#include "../constants.hpp"

GameStateChallenge::GameStateChallenge(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) :
    theme(renderer, options, Theme::NONE),
    pause_screen(renderer, fonts, options, _("Game Paused"), _("press the confirm button to exit")),
    win_screen(renderer, fonts, options, _("Level Finished!"), _("press the confirm button to continue")),
    lose_screen(renderer, fonts, options, _("Level Failed, continue?"), _("press the confirm button to try again"))
{
    this->renderer = renderer;
    this->fonts = fonts;
    this->sounds = sounds;
    this->options = options;

    loadLevel();
}

GameStateChallenge::~GameStateChallenge() {
    delete(this->board);
}

void GameStateChallenge::update() {
    if (this->board->isCompleted() && !this->completed) {
        this->theme.pause();
        this->sounds->play(Sound::COMPLETED);
        this->completed = true;
    } else if (!this->board->hasMovesLeft() && !this->failed && !this->completed) {
        this->theme.pause();
        this->sounds->play(Sound::FAILED);
        this->failed = true;
    }

    if (!this->completed &&  !this->failed) {
        this->theme.update();
        if(!this->paused) {
            this->board->update();
        }
    }
}

void GameStateChallenge::handleEvents(std::vector<Event> events) {
    if (!this->paused && !this->completed &&  !this->failed) {
        this->board->handleEvents(events);
    }

    for(Event event: events) {
        if (event == Event::QUIT) {
            this->options->setChallengeModeLevel(this->level);
            this->next_state = State::EXIT;
            this->done = true;
            return;
        } else if (event == Event::WINDOW_RESIZE) {
            this->pause_screen.updateSizing();
            this->win_screen.updateSizing();
            this->lose_screen.updateSizing();
            this->board->updateSizing();
        }
        if (this->completed) {
            if (event == Event::CONFIRM) {
                this->completed = false;
                this->theme.next();
                this->level += 1;
                this->seed += 1;
                if (this->level % 5 == 0 && this->required_matches < CHALLENGE_MODE_MAX_REQUIRED_MATCHES) {
                    this->required_matches += 1;
                }
                if (this->level > this->options->getChallengeModeHighscore()) {
                    this->options->setChallengeModeHighscore(this->level);
                }
                this->board->loadLevel(this->width, this->height, this->moves, this->required_matches, this->level, this->seed);
            }
        } else if (this->failed) {
            if (event == Event::CONFIRM) {
                this->failed = false;
                this->board->reset();
                this->theme.unpause();
            } else if (event == Event::CANCEL) {
                if(this->options->getChallengeModeHighscore() < this->level) {
                    this->options->setChallengeModeHighscore(this->level);
                }
                this->options->resetChallengeMode();
                this->next_state = State::HIGHSCORES;
                this->done = true;
            }
        } else if (this->paused) {
            if (event == Event::CONFIRM) {
                this->options->setChallengeModeLevel(this->level);
                this->done = true;
            } else if (event == Event::CANCEL || event == Event::MENU) {
                this->paused = false;
            }
        } else if (event == Event::MENU) {
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
        this->board->draw(renderer);
    }
}

void GameStateChallenge::loadLevel() {
    this->width = DEFAULT_BOARD_WIDTH;
    this->height = DEFAULT_BOARD_HEIGHT;

    this->moves = 10;
    this->level = this->options->getChallengeModeLevel();
    this->required_matches = 11 + (this->level / 5);
    if (this->required_matches > CHALLENGE_MODE_MAX_REQUIRED_MATCHES) {
        this->required_matches = CHALLENGE_MODE_MAX_REQUIRED_MATCHES;
    }
    if (this->level == 1) {
        srand(SDL_GetTicks());
        this->options->setChallengeModeSeed(rand());
        this->seed = this->options->getChallengeModeSeed();
    } else {
        this->seed = this->options->getChallengeModeSeed() + level;
    }

    this->board = new BoardManager(
        renderer,
        this->fonts,
        this->sounds,
        this->options,
        this->width,
        this->height,
        this->moves,
        this->required_matches,
        this->level,
        this->seed
    );

    this->theme.switchTheme(this->level - 1);
}

State GameStateChallenge::getNextState() {
    return this->next_state;
}
