#include "HowToMatchState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "../FontType.hpp"
#include "../utils.hpp"
#include "../ButtonImage.hpp"
#include "../TextureManager.hpp"
#include "GameState.hpp"

HowToMatchState::HowToMatchState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) : renderer(renderer), fonts(fonts), sounds(sounds), options(options),
    theme(renderer, options, Theme::MENU), textures(renderer, options)
{
    this->loadTexts();
}

HowToMatchState::~HowToMatchState() {
    SDL_DestroyTexture(text_title);
    SDL_DestroyTexture(text_bottom);
    SDL_DestroyTexture(text_match_shells);
    SDL_DestroyTexture(text_conclusion);
    SDL_DestroyTexture(text_match);
}


void HowToMatchState::handleEvents(std::vector<Event> events) {
    for(Event event :events) {
        switch (event) {
            case Event::QUIT:
                this->next_state = State::EXIT;
                this->done = true;
                break;
            case Event::CANCEL:
                this->next_state = State::HOWTOPLAY;
                this->done = true;
                break;
            case Event::MENU:
                this->next_state = State::MENU;
                this->done = true;
                break;
            case Event::CONFIRM:
                this->done = true;
                break;
            case Event::WINDOW_RESIZE:
                this->updateSizing();
                break;
            default:
                break;
        }
    }
    
}

void HowToMatchState::update() {
    this->theme.update();
}

void HowToMatchState::draw(SDL_Renderer * renderer) {
    this->theme.draw(renderer);

    loadTexts();

    // Draw title
    SDL_Rect rect_title_text = {this->options->getScreenWidth() / 2, this->options->getScreenHeight() / 8, 0, 0};
    SDL_QueryTexture(this->text_title, NULL, NULL, &rect_title_text.w, &rect_title_text.h);
    rect_title_text.x -= rect_title_text.w / 2;
    rect_title_text.y -= rect_title_text.h / 2;
    SDL_RenderCopy(renderer, this->text_title, NULL, &rect_title_text);

    // Draw bottom text
    SDL_Rect rect_bottom_text = {this->options->getScreenWidth() / 2, this->options->getScreenHeight(), 0, 0};
    SDL_QueryTexture(this->text_bottom, NULL, NULL, &rect_bottom_text.w, &rect_bottom_text.h);
    rect_bottom_text.x -= rect_bottom_text.w / 2;
    rect_bottom_text.y -= rect_bottom_text.h * 1.5;
    SDL_RenderCopy(renderer, this->text_bottom, NULL, &rect_bottom_text);

    // Draw background
    SDL_Rect rect_background;
    rect_background.x = 0;
    rect_background.y = this->options->getScreenHeight()/4;
    rect_background.w = this->options->getScreenWidth();
    rect_background.h = rect_bottom_text.y - rect_background.y;
    SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
    SDL_RenderFillRect(renderer, &rect_background);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderDrawLine(renderer,
        0,
        rect_background.y,
        this->options->getScreenWidth(),
        rect_background.y
    );
    SDL_RenderDrawLine(renderer,
        0,
        rect_background.y + rect_background.h,
        this->options->getScreenWidth(),
        rect_background.y + rect_background.h
    );

    // Draw match line
    SDL_Rect rect_match = {this->options->getScreenWidth() / 2, this->text_start_y, 0, 0};
    SDL_QueryTexture(this->text_match, NULL, NULL, &rect_match.w, &rect_match.h);
    rect_match.x -= rect_match.w / 2;
    SDL_RenderCopy(renderer, this->text_match, NULL, &rect_match);

    //Draw grid
    int grid_size = 3;
    SDL_Point middle = {this->options->getScreenWidth() / 2, (rect_bottom_text.y + this->text_start_y) / 2};
    SDL_Rect grid = {middle.x - (int)(this->options->getShellSize() * 1.5), middle.y - (int)(this->options->getShellSize() * 1.5), this->options->getShellSize() * grid_size, this->options->getShellSize() * grid_size};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < 4; i++) {
        SDL_RenderDrawLine(renderer,
            grid.x,
            grid.y + this->options->getShellSize() * i,
            grid.x + grid.w,
            grid.y + this->options->getShellSize() * i
        );
        SDL_RenderDrawLine(renderer,
            grid.x + this->options->getShellSize() * i,
            grid.y,
            grid.x + this->options->getShellSize() * i,
            grid.y + grid.h
        );
    }

    // Draw shells
    SDL_Rect rect_shell_src = {0, 0, this->options->getShellSize(), this->options->getShellSize()};
    SDL_Rect rect_shell_dest = {grid.x + this->options->getShellSize() * 2, grid.y, rect_shell_src.w, rect_shell_src.h};
    SDL_RenderCopy(renderer, textures.getShellTexture(), &rect_shell_src, &rect_shell_dest);
    rect_shell_dest.x -= this->options->getShellSize() * 2;
    rect_shell_dest.y += this->options->getShellSize();
    SDL_RenderCopy(renderer, textures.getShellTexture(), &rect_shell_src, &rect_shell_dest);
    rect_shell_dest.y += this->options->getShellSize();
    SDL_RenderCopy(renderer, textures.getShellTexture(), &rect_shell_src, &rect_shell_dest);

    //  Draw arrow
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer,
        grid.x + this->options->getShellSize() / 2,
        grid.y + this->options->getShellSize() / 2,
        grid.x + this->options->getShellSize() / 2 + this->options->getShellSize() * 2,
        grid.y + this->options->getShellSize() / 2
    );
    SDL_RenderDrawLine(renderer,
        grid.x + this->options->getShellSize() / 2,
        grid.y + this->options->getShellSize() / 2,
        grid.x + this->options->getShellSize() - this->options->getShellSize() / 4,
        grid.y + this->options->getShellSize() - this->options->getShellSize() / 4
    );

    SDL_RenderDrawLine(renderer,
        grid.x + this->options->getShellSize() / 2,
        grid.y + this->options->getShellSize() / 2,
        grid.x + this->options->getShellSize() - this->options->getShellSize() / 4,
        grid.y + this->options->getShellSize() / 4
    );

    // Draw conclusion
    SDL_Rect rect_conclusion = {this->options->getScreenWidth() / 2, rect_bottom_text.y, 0, 0};
    SDL_QueryTexture(this->text_conclusion, NULL, NULL, &rect_conclusion.w, &rect_conclusion.h);
    rect_conclusion.x -= rect_conclusion.w / 2;
    rect_conclusion.y -= rect_conclusion.h + this->options->getShellSize() / 4;
    SDL_RenderCopy(renderer, this->text_conclusion, NULL, &rect_conclusion);
}

void HowToMatchState::loadTexts() {
    if (!this->text_title) {
        this->text_title = fonts->getTexture(renderer, _("How to Match"), FontType::TITLE, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});
    }
    if (!this->text_bottom) {
        this->text_bottom = fonts->getTexture(renderer, _("press confirm to continue"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_match) {
        this->text_match = fonts->getTexture(renderer, _("3 shells make a match"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_conclusion) {
        this->text_conclusion = fonts->getTexture(renderer, _("moves without a match fail"), FontType::NORMAL, {255, 255, 255, 255});
    }
    this->text_start_y = this->options->getScreenHeight() / 4;
}

void HowToMatchState::updateSizing() {
    if (this->text_title) {
        SDL_DestroyTexture(this->text_title);
        this->text_title = NULL;
    }
    if (this->text_bottom) {
        SDL_DestroyTexture(this->text_bottom);
        this->text_bottom = NULL;
    }
    if (this->text_match_shells) {
        SDL_DestroyTexture(this->text_match_shells);
        this->text_match_shells = NULL;
    }
    if (this->text_conclusion) {
        SDL_DestroyTexture(this->text_conclusion);
        this->text_conclusion = NULL;
    }
    if (this->text_match) {
        SDL_DestroyTexture(this->text_match);
        this->text_match = NULL;
    }
    this->textures.reload();
}

State HowToMatchState::getNextState() {
    return this->next_state;
}