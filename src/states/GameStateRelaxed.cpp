#include "GameStateRelaxed.hpp"

GameStateRelaxed::GameStateRelaxed(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) :
    theme(renderer, options, Theme::MENU),
    pause_screen(renderer, fonts, options, "Game Paused", "Press the confirm button to exit")
{
    this->renderer = renderer;
    this->fonts = fonts;
    this->sounds = sounds;
    this->options = options;

    // Set a random theme
    srand(SDL_GetTicks());
    theme.switchTheme((rand() % ((int) Theme::AMOUNT - 1)) + 1);

    loadLevel();
}

GameStateRelaxed::~GameStateRelaxed() {
    delete(this->board);
}

void GameStateRelaxed::update() {
    if (!this->paused) {
        this->theme.update();
        this->board->update();
    }
}

void GameStateRelaxed::handleEvents(std::vector<Event> events) {
    if (!this->paused) {
        this->board->handleEvents(events);
    }

    for(Event event: events) {
        if (event == Event::MENU) {
            this->paused = !(this->paused);
            return;
        } else if (event == Event::QUIT) {
            this->paused = true;
            return;
        }

        if (this->paused) {
            if (event == Event::CONFIRM || event == Event::QUIT) {
                this->options->setRelaxedModeScore(this->board->getMatches());
                this->options->setRelaxedModeShells(this->board->getCurrentShells());
                this->options->setRelaxedModeSeed(this->board->getCurrentSeed());
                this->done = true;
            } else if (event == Event::CANCEL || event == Event::MENU) {
                this->paused = false;
            }
        }
    }
}

void GameStateRelaxed::draw(SDL_Renderer *renderer) {
    this->theme.draw(renderer);
    if (this->paused) {
        pause_screen.draw(renderer);
    } else {
        this->board->draw(renderer);
    }
}

void GameStateRelaxed::loadLevel() {
    this->width = DEFAULT_BOARD_WIDTH;
    this->height = DEFAULT_BOARD_HEIGHT;

    this->position = calculatePosition(this->width, this->height);

    int moves = 0;
    int required_matches = 0;
    int level = 0;
    int seed = this->options->getRelaxedModeSeed();

    if (seed == 0) {
        this->board = new BoardManager(
            renderer,
            this->fonts,
            this->sounds,
            this->options,
            this->position.x,
            this->position.y,
            this->width,
            this->height,
            moves,
            required_matches,
            level,
            seed
        );
    } else {
        this->board = new BoardManager(
            renderer,
            this->fonts,
            this->sounds,
            this->options,
            this->position.x,
            this->position.y,
            this->options->getRelaxedModeShells(),
            moves,
            required_matches,
            level,
            seed
        );
    }

    this->board->setMatches(this->options->getRelaxedModeScore());
}

SDL_Point GameStateRelaxed::calculatePosition(int width, int height) {
    return {(this->options->getScreenWidth()-this->options->getShellSize()*width)/2, (this->options->getScreenHeight()-this->options->getShellSize()*(height+1))/2};
}

State GameStateRelaxed::getNextState() {
    return State::MENU;
}
