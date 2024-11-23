#include "HowToState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "../FontType.hpp"
#include "../utils.hpp"
#include "../ButtonImage.hpp"
#include "../TextureManager.hpp"
#include "GameState.hpp"

HowToState::HowToState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) : renderer(renderer), fonts(fonts), sounds(sounds), options(options),
    theme(renderer, options, Theme::MENU)
{
    this->loadTexts();
    this->loadImages();
}

HowToState::~HowToState() {
    SDL_DestroyTexture(text_title);
    for (int i = 0; i < (int) texts.size(); i++) {
        SDL_DestroyTexture(texts[i]);
        texts[i] = NULL;
    }
}


void HowToState::handleEvents(std::vector<Event> events) {
    for(Event event :events) {
        switch (event) {
            case Event::QUIT:
                this->next_state = State::EXIT;
                this->done = true;
                break;
            case Event::MENU:
            case Event::CANCEL:
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

void HowToState::update() {
    this->theme.update();
}

void HowToState::draw(SDL_Renderer * renderer) {
    this->theme.draw(renderer);

    if (this->texts.empty()) {
        loadTexts();
    }

    // Draw title
    SDL_Rect rect_title = {this->options->getScreenWidth() / 2, this->options->getScreenHeight() / 8, 0, 0};
    SDL_QueryTexture(text_title, NULL, NULL, &rect_title.w, &rect_title.h);
    rect_title.x -= rect_title.w/2;
    rect_title.y -= rect_title.h/2;
    SDL_RenderCopy(renderer, text_title, NULL, &rect_title);

    SDL_Rect rect_arrows = {0, 0, this->options->getShellSize(), this->options->getShellSize()};
    SDL_RenderCopy(renderer, this->images[(int) ButtonImage::KEYBOARD_MOVEMENT], NULL, &rect_arrows);

    // Draw options
    for(int i = 0; i < (int) texts.size(); i++) {
        // Draw the option title
        SDL_Rect rect = {this->options->getScreenWidth()/2, getTextY(i), 0, 0};
        SDL_QueryTexture(texts[i], NULL, NULL, &rect.w, &rect.h);
        rect.x -= rect.w/2;

        // Render the option text
        SDL_RenderCopy(renderer, texts[i], NULL, &rect);
    }

    SDL_Rect rect_bottom = {this->options->getScreenWidth()/2, getTextY(((int) texts.size()) + 1), 0, 0};
    SDL_QueryTexture(text_bottom, NULL, NULL, &rect_bottom.w, &rect_bottom.h);
    rect_bottom.x -= rect_bottom.w/2;
    SDL_RenderCopy(renderer, text_bottom, NULL, &rect_bottom);
}

void HowToState::loadTexts() {
    this->text_title = fonts->getTexture(renderer, _("How to Play"), FontType::TITLE, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});
    this->text_bottom = fonts->getTexture(renderer, _("press confirm to continue"), FontType::NORMAL, {255, 255, 255, 255});
    this->slash = fonts->getTexture(renderer, "/", FontType::NORMAL, {255, 255, 255, 255});

    this->texts.push_back(fonts->getTexture(renderer, _("standard mode completed: "), FontType::NORMAL, {255, 255, 255, 255}));
    this->texts.push_back(fonts->getTexture(renderer, _("highest level reached in challenge mode: ") + std::to_string(options->getChallengeModeHighscore()), FontType::NORMAL, {255, 255, 255, 255}));
    this->texts.push_back(fonts->getTexture(renderer, _("matches in relaxed mode: ") + std::to_string(options->getRelaxedModeScore()), FontType::NORMAL, {255, 255, 255, 255}));

    this->text_start_y = getTextY(0);
}

void HowToState::loadImages() {
    if (this->images.empty()) {
        for (int i = 0; i < (int) ButtonImage::AMOUNT; i++) {
            this->images.push_back(NULL);
        }
    }
    for (int i = 0; i < (int) ButtonImage::AMOUNT; i++) {
        switch ((ButtonImage) i)
        {
        case ButtonImage::KEYBOARD_MOVEMENT:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/keyboard_arrows.svg");
            break;
        case ButtonImage::KEYBOARD_CONFIRM:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/keyboard_space.svg");
            break;
        case ButtonImage::KEYBOARD_CANCEL:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/keyboard_backspace.svg");
            break;
        case ButtonImage::KEYBOARD_MENU:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/keyboard_escape.svg");
            break;
        case ButtonImage::MOUSE_MOVEMENT:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/mouse.svg");
            break;
        case ButtonImage::MOUSE_CONFIRM:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/mouse_left.svg");
            break;
        case ButtonImage::MOUSE_CANCEL:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/mouse_right.svg");
            break;
        case ButtonImage::MOUSE_MENU:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/mouse_scroll.svg");
            break;
        case ButtonImage::GAMEPAD_MOVEMENT:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/switch_dpad.svg");
            break;
        case ButtonImage::GAMEPAD_CONFIRM:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/switch_buttons_down.svg");
            break;
        case ButtonImage::GAMEPAD_CANCEL:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/switch_buttons_right.svg");
            break;
        case ButtonImage::GAMEPAD_MENU:
            this->images[i] = TextureManager::load(this->renderer, "assets/images/steam_button_start_icon.svg");
            break;
        default:
            break;
        }
    }
}

void HowToState::updateSizing() {
    for (size_t i = 0; i < texts.size(); i++) {
        SDL_DestroyTexture(texts[i]);
        texts[i] = NULL;
    }
    if (this->text_title) {
        SDL_DestroyTexture(this->text_title);
        this->text_title = NULL;
    }
    if (this->text_bottom) {
        SDL_DestroyTexture(this->text_bottom);
        this->text_bottom = NULL;
    }
    this->texts.clear();
}

int HowToState::getTextY(int number) {
    return this->options->getScreenHeight()/(((int) texts.size())+this->text_offset*2)*(number+this->text_offset);
}

State HowToState::getNextState() {
    return this->next_state;
}