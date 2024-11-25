#include "HowToPlayState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "../FontType.hpp"
#include "../utils.hpp"
#include "../ButtonImage.hpp"
#include "../TextureManager.hpp"
#include "GameState.hpp"

HowToPlayState::HowToPlayState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) : renderer(renderer), fonts(fonts), sounds(sounds), options(options),
    theme(renderer, options, Theme::MENU), textures(renderer, options)
{
    this->loadTexts();
}

HowToPlayState::~HowToPlayState() {
    SDL_DestroyTexture(text_title);
    SDL_DestroyTexture(text_bottom);
    SDL_DestroyTexture(text_match_shells);
    SDL_DestroyTexture(text_match_bubbles);
    SDL_DestroyTexture(text_match_urchins);
    SDL_DestroyTexture(text_goal);
}


void HowToPlayState::handleEvents(std::vector<Event> events) {
    for(Event event :events) {
        switch (event) {
            case Event::QUIT:
                this->next_state = State::EXIT;
                this->done = true;
                break;
            case Event::CANCEL:
                this->next_state = State::HOWTOCONTROLS;
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

void HowToPlayState::update() {
    this->theme.update();
}

void HowToPlayState::draw(SDL_Renderer * renderer) {
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

    // Draw everything else
    SDL_Rect rect_goal = {this->options->getScreenWidth() / 2, this->text_start_y, 0, 0};
    SDL_QueryTexture(this->text_goal, NULL, NULL, &rect_goal.w, &rect_goal.h);
    rect_goal.x -= rect_goal.w / 2;
    SDL_RenderCopy(renderer, this->text_goal, NULL, &rect_goal);

    SDL_Rect rect_match_shells = {this->options->getScreenWidth() / 2, rect_goal.y + rect_goal.h, 0, 0};
    SDL_QueryTexture(this->text_match_shells, NULL, NULL, &rect_match_shells.w, &rect_match_shells.h);
    rect_match_shells.x -= rect_match_shells.w / 2;
    SDL_RenderCopy(renderer, this->text_match_shells, NULL, &rect_match_shells);

    SDL_Rect rect_shells_src = {0, 0, this->options->getShellSize() * 5, this->options->getShellSize()};
    SDL_Rect rect_shells_dest = {this->options->getScreenWidth() /2 - this->options->getShellSize() * 2.5, rect_match_shells.y + rect_match_shells.h, rect_shells_src.w, rect_shells_src.h};
    SDL_RenderCopy(renderer, textures.getShellTexture(), &rect_shells_src, &rect_shells_dest);

    SDL_Rect rect_match_bubbles = {this->options->getScreenWidth() / 2, rect_shells_dest.y + rect_shells_dest.h, 0, 0};
    SDL_QueryTexture(this->text_match_bubbles, NULL, NULL, &rect_match_bubbles.w, &rect_match_bubbles.h);
    rect_match_bubbles.x -= rect_match_bubbles.w / 2;
    SDL_RenderCopy(renderer, this->text_match_bubbles, NULL, &rect_match_bubbles);

    SDL_Rect rect_bubble_src = {this->options->getShellSize() * 6, 0, this->options->getShellSize(), this->options->getShellSize()};
    SDL_Rect rect_bubble_dest = {this->options->getScreenWidth() / 2 - this->options->getShellSize() / 2, rect_match_bubbles.y + rect_match_bubbles.h, rect_bubble_src.w, rect_bubble_src.h};
    SDL_RenderCopy(renderer, textures.getShellTexture(), &rect_bubble_src, &rect_bubble_dest);

    SDL_Rect rect_match_urchins = {this->options->getScreenWidth() / 2, rect_bubble_dest.y + rect_bubble_dest.h, 0, 0};
    SDL_QueryTexture(this->text_match_urchins, NULL, NULL, &rect_match_urchins.w, &rect_match_urchins.h);
    rect_match_urchins.x -= rect_match_urchins.w / 2;
    SDL_RenderCopy(renderer, this->text_match_urchins, NULL, &rect_match_urchins);

    SDL_Rect rect_urchin_src = {this->options->getShellSize() * 5, 0, this->options->getShellSize(), this->options->getShellSize()};
    SDL_Rect rect_urchin_dest = {this->options->getScreenWidth() / 2 - this->options->getShellSize() / 2, rect_match_urchins.y + rect_match_urchins.h, rect_urchin_src.w, rect_urchin_src.h};
    if (rect_urchin_dest.y + rect_urchin_dest.h < rect_bottom_text.y) {
        SDL_RenderCopy(renderer, textures.getShellTexture(), &rect_urchin_src, &rect_urchin_dest);
    }
}

void HowToPlayState::loadTexts() {
    if (!this->text_title) {
        this->text_title = fonts->getTexture(renderer, _("How to Play"), FontType::TITLE, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});
    }
    if (!this->text_bottom) {
        this->text_bottom = fonts->getTexture(renderer, _("press confirm to continue"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_goal) {
        this->text_goal = fonts->getTexture(renderer, _("reach the score with limited moves"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_match_shells) {
        this->text_match_shells = fonts->getTexture(renderer, _("match these to score points"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_match_bubbles) {
        this-> text_match_bubbles = fonts->getTexture(renderer, _("match bubbles for extra moves"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_match_urchins) {
        this->text_match_urchins = fonts->getTexture(renderer, _("never match urchins - they hurt!"), FontType::NORMAL, {255, 255, 255, 255});
    }
    this->text_start_y = this->options->getScreenHeight() / 4;
}

void HowToPlayState::updateSizing() {
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
    if (this->text_match_bubbles) {
        SDL_DestroyTexture(this->text_match_bubbles);
        this->text_match_bubbles = NULL;
    }
    if (this->text_match_urchins) {
        SDL_DestroyTexture(this->text_match_urchins);
        this->text_match_urchins = NULL;
    }
    if (this->text_goal) {
        SDL_DestroyTexture(this->text_goal);
        this->text_goal = NULL;
    }
    this->textures.reload();
}

State HowToPlayState::getNextState() {
    return this->next_state;
}