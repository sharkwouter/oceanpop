#include "HowToMatchState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "../FontType.hpp"
#include "../utils.hpp"
#include "../ButtonImage.hpp"
#include "../TextureManager.hpp"
#include "GameState.hpp"

HowToMatchState::HowToMatchState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) : renderer(renderer), fonts(fonts), sounds(sounds), options(options),
    theme(renderer, sounds->getMixer(), options, Theme::MENU), textures(renderer, options)
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
            case Event::LEFT:
            case Event::CANCEL:
                this->next_state = State::HOWTOPLAY;
                this->done = true;
                break;
            case Event::MENU:
            case Event::RIGHT:
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
    SDL_FRect rect_title_text = {(float) this->options->getScreenWidth() / 2.0f, (float) this->options->getScreenHeight() / 8.0f, 0.0f, 0.0f};
    SDL_GetTextureSize(this->text_title, &rect_title_text.w, &rect_title_text.h);
    rect_title_text.x -= rect_title_text.w / 2.0f;
    rect_title_text.y -= rect_title_text.h / 2.0f;
    SDL_RenderTexture(renderer, this->text_title, NULL, &rect_title_text);

    // Draw bottom text
    SDL_FRect rect_bottom_text = {(float) this->options->getScreenWidth() / 2.0f, (float) this->options->getScreenHeight(), 0.0f, 0.0f};
    SDL_GetTextureSize(this->text_bottom, &rect_bottom_text.w, &rect_bottom_text.h);
    rect_bottom_text.x -= rect_bottom_text.w / 2.0f;
    rect_bottom_text.y -= rect_bottom_text.h * 1.5f;
    SDL_RenderTexture(renderer, this->text_bottom, NULL, &rect_bottom_text);

    // Draw background
    SDL_FRect rect_background;
    rect_background.x = 0.0f;
    rect_background.y = (float) this->options->getScreenHeight() / 4.0f;
    rect_background.w = (float) this->options->getScreenWidth();
    rect_background.h = rect_bottom_text.y - rect_background.y;
    SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
    SDL_RenderFillRect(renderer, &rect_background);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderLine(renderer,
        0,
        rect_background.y,
        this->options->getScreenWidth(),
        rect_background.y
    );
    SDL_RenderLine(renderer,
        0,
        rect_background.y + rect_background.h,
        this->options->getScreenWidth(),
        rect_background.y + rect_background.h
    );

    // Draw match line
    SDL_FRect rect_match = {(float) this->options->getScreenWidth() / 2.0f, (float) this->text_start_y, 0.0f, 0.0f};
    SDL_GetTextureSize(this->text_match, &rect_match.w, &rect_match.h);
    rect_match.x -= rect_match.w / 2.0f;
    SDL_RenderTexture(renderer, this->text_match, NULL, &rect_match);

    //Draw grid
    int grid_size = 3;
    SDL_FPoint middle = {(float) this->options->getScreenWidth() / 2.0f, ((float) rect_bottom_text.y + (float) this->text_start_y) / 2.0f};
    SDL_FRect grid = {middle.x - (float)(this->options->getShellSize() * 1.5), middle.y - (float)(this->options->getShellSize() * 1.5), (float)this->options->getShellSize() * (float) grid_size, (float)(this->options->getShellSize() * (float) grid_size)};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    for (int i = 0; i < 4; i++) {
        SDL_RenderLine(renderer,
            grid.x,
            grid.y + this->options->getShellSize() * i,
            grid.x + grid.w,
            grid.y + this->options->getShellSize() * i
        );
        SDL_RenderLine(renderer,
            grid.x + this->options->getShellSize() * i,
            grid.y,
            grid.x + this->options->getShellSize() * i,
            grid.y + grid.h
        );
    }

    // Draw shells
    SDL_FRect rect_shell_src = {0.0f, 0.0f, (float) this->options->getShellSize(), (float) this->options->getShellSize()};
    SDL_FRect rect_shell_dest = {grid.x + (float) this->options->getShellSize() * 2.0f, grid.y, rect_shell_src.w, rect_shell_src.h};
    SDL_RenderTexture(renderer, textures.getShellTexture(), &rect_shell_src, &rect_shell_dest);
    rect_shell_dest.x -= (float) this->options->getShellSize() * 2.0f;
    rect_shell_dest.y += (float) this->options->getShellSize();
    SDL_RenderTexture(renderer, textures.getShellTexture(), &rect_shell_src, &rect_shell_dest);
    rect_shell_dest.y += (float) this->options->getShellSize();
    SDL_RenderTexture(renderer, textures.getShellTexture(), &rect_shell_src, &rect_shell_dest);

    //  Draw arrow
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderLine(renderer,
        grid.x + this->options->getShellSize() / 2,
        grid.y + this->options->getShellSize() / 2,
        grid.x + grid.w - this->options->getShellSize() / 2,
        grid.y + this->options->getShellSize() / 2
    );
    SDL_RenderLine(renderer,
        grid.x + this->options->getShellSize() / 2,
        grid.y + this->options->getShellSize() / 2,
        grid.x + this->options->getShellSize() - this->options->getShellSize() / 4,
        grid.y + this->options->getShellSize() - this->options->getShellSize() / 4
    );

    SDL_RenderLine(renderer,
        grid.x + this->options->getShellSize() / 2,
        grid.y + this->options->getShellSize() / 2,
        grid.x + this->options->getShellSize() - this->options->getShellSize() / 4,
        grid.y + this->options->getShellSize() / 4
    );

    // Draw conclusion
    SDL_FRect rect_conclusion = {(float) this->options->getScreenWidth() / 2.0f, (float) rect_bottom_text.y, 0.0f, 0.0f};
    SDL_GetTextureSize(this->text_conclusion, &rect_conclusion.w, &rect_conclusion.h);
    rect_conclusion.x -= rect_conclusion.w / 2.0f;
    rect_conclusion.y -= rect_conclusion.h + (float) this->options->getShellSize() / 4.0f;
    SDL_RenderTexture(renderer, this->text_conclusion, NULL, &rect_conclusion);
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