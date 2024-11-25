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
    SDL_DestroyTexture(text_plus_one);
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

    // Draw conclusion
    SDL_Rect rect_conclusion = {this->options->getScreenWidth() / 2, rect_bottom_text.y, 0, 0};
    SDL_QueryTexture(this->text_conclusion, NULL, NULL, &rect_conclusion.w, &rect_conclusion.h);
    rect_conclusion.x -= rect_conclusion.w / 2;
    rect_conclusion.y -= rect_conclusion.h + this->options->getShellSize() / 4;
    SDL_RenderCopy(renderer, this->text_conclusion, NULL, &rect_conclusion);

    // Draw everything else
    SDL_Rect rect_match = {this->options->getScreenWidth() / 2, this->text_start_y, 0, 0};
    SDL_QueryTexture(this->text_match, NULL, NULL, &rect_match.w, &rect_match.h);
    rect_match.x -= rect_match.w / 2;
    SDL_RenderCopy(renderer, this->text_match, NULL, &rect_match);

    // Draw first +1
    SDL_Rect rect_plus_one = {this->options->getScreenWidth() / 2, this->options->getScreenHeight() / 2 - this->options->getShellSize() * 1.5, 0, 0};
    SDL_QueryTexture(this->text_plus_one, NULL, NULL, &rect_plus_one.w, &rect_plus_one.h);
    rect_plus_one.x = rect_plus_one.x - rect_plus_one.w / 2 + this->options->getShellSize() / 2;
    SDL_RenderCopy(renderer, this->text_plus_one, NULL, &rect_plus_one);

    // Draw match lines top
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w / 2 - this->options->getShellSize(),
        rect_plus_one.y + rect_plus_one.h + this->options->getShellSize() / 4,
        rect_plus_one.x + rect_plus_one.w / 2  + this->options->getShellSize(),
        rect_plus_one.y + rect_plus_one.h + this->options->getShellSize() / 4
    );
    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w / 2 - this->options->getShellSize(),
        rect_plus_one.y + rect_plus_one.h + this->options->getShellSize() / 4,
        rect_plus_one.x + rect_plus_one.w / 2 - this->options->getShellSize(),
        rect_plus_one.y + rect_plus_one.h + this->options->getShellSize() / 2
    );
    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w / 2  + this->options->getShellSize(),
        rect_plus_one.y + rect_plus_one.h + this->options->getShellSize() / 4,
        rect_plus_one.x + rect_plus_one.w / 2  + this->options->getShellSize(),
        rect_plus_one.y + rect_plus_one.h + this->options->getShellSize() / 2
    );

    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w / 2,
        rect_plus_one.y + rect_plus_one.h,
        rect_plus_one.x + rect_plus_one.w / 2,
        rect_plus_one.y + rect_plus_one.h + this->options->getShellSize() / 4
    );

    // Draw horizontal shells
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect rect_shell_src = {0, 0, this->options->getShellSize(), this->options->getShellSize()};
    SDL_Rect shells_h[4];
    for (int i = 0; i < 4; i++) {
        shells_h[i] = {this->options->getScreenWidth() / 2 - this->options->getShellSize() + this->options->getShellSize() * i, rect_plus_one.y + rect_plus_one.h + this->options->getShellSize() / 2, rect_shell_src.w, rect_shell_src.h};
        SDL_RenderCopy(renderer, textures.getShellTexture(), &rect_shell_src, &shells_h[i]);
        SDL_RenderDrawLine(renderer,
            shells_h[i].x,
            shells_h[i].y,
            shells_h[i].x,
            shells_h[i].y + shells_h[i].h
        );
        if (i == 3) {
            SDL_RenderDrawLine(renderer,
                shells_h[i].x + shells_h[i].w,
                shells_h[i].y,
                shells_h[i].x + shells_h[i].w,
                shells_h[i].y + shells_h[i].h
            );
        }
    }
    SDL_RenderDrawLine(renderer,
        shells_h[0].x,
        shells_h[0].y,
        shells_h[3].x + shells_h[3].w,
        shells_h[0].y
    );
    SDL_RenderDrawLine(renderer,
        shells_h[0].x,
        shells_h[0].y + shells_h[0].h,
        shells_h[3].x + shells_h[3].w,
        shells_h[0].y + shells_h[0].h
    );

    // Draw second +1
    rect_plus_one.x += this->options->getShellSize();
    rect_plus_one.y = shells_h[0].y + shells_h[0].h + this->options->getShellSize() / 2;
    SDL_RenderCopy(renderer, this->text_plus_one, NULL, &rect_plus_one);

    // Draw match lines bottom
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w / 2 - this->options->getShellSize(),
        rect_plus_one.y - this->options->getShellSize() / 4,
        rect_plus_one.x + rect_plus_one.w / 2  + this->options->getShellSize(),
        rect_plus_one.y - this->options->getShellSize() / 4
    );
    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w / 2 - this->options->getShellSize(),
        rect_plus_one.y - this->options->getShellSize() / 4,
        rect_plus_one.x + rect_plus_one.w / 2 - this->options->getShellSize(),
        rect_plus_one.y - this->options->getShellSize() / 2 + 1
    );
    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w / 2  + this->options->getShellSize(),
        rect_plus_one.y - this->options->getShellSize() / 4,
        rect_plus_one.x + rect_plus_one.w / 2  + this->options->getShellSize(),
        rect_plus_one.y - this->options->getShellSize() / 2 + 1
    );

    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w / 2,
        rect_plus_one.y,
        rect_plus_one.x + rect_plus_one.w / 2,
        rect_plus_one.y - this->options->getShellSize() / 4
    );

    // Draw vertical shells
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect shells_v[2];
    for (int i = 0; i < 2; i++) {
        shells_v[i] = {this->options->getScreenWidth() / 2 - this->options->getShellSize(), shells_h[0].y + shells_h[0].h + this->options->getShellSize() * i, rect_shell_src.w, rect_shell_src.h};
        SDL_RenderCopy(renderer, textures.getShellTexture(), &rect_shell_src, &shells_v[i]);
        SDL_RenderDrawLine(renderer,
            shells_v[i].x,
            shells_v[i].y + shells_v[i].h,
            shells_v[i].x + shells_v[i].w,
            shells_v[i].y + shells_v[i].h
        );
    }
    SDL_RenderDrawLine(renderer,
        shells_v[0].x,
        shells_v[0].y,
        shells_v[0].x,
        shells_v[1].y + shells_v[1].h
    );
    SDL_RenderDrawLine(renderer,
        shells_v[0].x + shells_v[0].w,
        shells_v[0].y,
        shells_v[0].x + shells_v[0].w,
        shells_v[1].y + shells_v[1].h
    );

    // Draw third +1
    rect_plus_one.x = shells_v[0].x - rect_plus_one.w - this->options->getShellSize() / 2;
    rect_plus_one.y = shells_v[0].y + this->options->getShellSize() / 2 - rect_plus_one.h / 2;
    SDL_RenderCopy(renderer, this->text_plus_one, NULL, &rect_plus_one);

    // Draw match lines bottom
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w + this->options->getShellSize() / 4,
        rect_plus_one.y + rect_plus_one.h / 2 - this->options->getShellSize(),
        rect_plus_one.x + rect_plus_one.w +  this->options->getShellSize() / 4,
        rect_plus_one.y + rect_plus_one.h / 2  + this->options->getShellSize()
    );
    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w + this->options->getShellSize() / 4,
        rect_plus_one.y + rect_plus_one.h / 2 - this->options->getShellSize(),
        rect_plus_one.x + rect_plus_one.w + this->options->getShellSize() / 2 - 1,
        rect_plus_one.y + rect_plus_one.h / 2 - this->options->getShellSize()
    );
    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w + this->options->getShellSize() / 4,
        rect_plus_one.y + rect_plus_one.h / 2 + this->options->getShellSize(),
        rect_plus_one.x + rect_plus_one.w + this->options->getShellSize() / 2 - 1,
        rect_plus_one.y + rect_plus_one.h / 2 + this->options->getShellSize()
    );

    SDL_RenderDrawLine(renderer,
        rect_plus_one.x + rect_plus_one.w,
        rect_plus_one.y + rect_plus_one.h / 2,
        rect_plus_one.x + rect_plus_one.w + this->options->getShellSize() / 4,
        rect_plus_one.y + rect_plus_one.h / 2
    );
}

void HowToMatchState::loadTexts() {
    if (!this->text_title) {
        this->text_title = fonts->getTexture(renderer, _("How Scoring Works"), FontType::TITLE, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});
    }
    if (!this->text_bottom) {
        this->text_bottom = fonts->getTexture(renderer, _("press confirm to continue"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_match) {
        this->text_match = fonts->getTexture(renderer, _("3 lined up shells gives a point"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_plus_one) {
        this-> text_plus_one = fonts->getTexture(renderer, _("+1"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_conclusion) {
        this->text_conclusion = fonts->getTexture(renderer, _("this example gives +3 points"), FontType::NORMAL, {255, 255, 255, 255});
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
    if (this->text_plus_one) {
        SDL_DestroyTexture(this->text_plus_one);
        this->text_plus_one = NULL;
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