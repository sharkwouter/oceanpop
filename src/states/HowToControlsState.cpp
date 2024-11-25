#include "HowToControlsState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "../FontType.hpp"
#include "../utils.hpp"
#include "../ButtonImage.hpp"
#include "../TextureManager.hpp"
#include "GameState.hpp"

HowToControlsState::HowToControlsState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) : renderer(renderer), fonts(fonts), sounds(sounds), options(options),
    theme(renderer, options, Theme::MENU)
{
    this->loadTexts();
    this->loadImages();
}

HowToControlsState::~HowToControlsState() {
    SDL_DestroyTexture(text_title);
    SDL_DestroyTexture(text_bottom);
    SDL_DestroyTexture(text_movement);
    SDL_DestroyTexture(text_confirm);
    SDL_DestroyTexture(text_cancel);
    SDL_DestroyTexture(text_menu);
}


void HowToControlsState::handleEvents(std::vector<Event> events) {
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

void HowToControlsState::update() {
    this->theme.update();
}

void HowToControlsState::draw(SDL_Renderer * renderer) {
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
    SDL_Rect rect_movement_text = {this->options->getScreenWidth() / 2, this->text_start_y, 0, 0};
    SDL_QueryTexture(this->text_movement, NULL, NULL, &rect_movement_text.w, &rect_movement_text.h);
    rect_movement_text.x -= rect_movement_text.w;
    rect_movement_text.y = rect_movement_text.y - rect_movement_text.h / 2 + this->options->getShellSize() / 2;
    SDL_RenderCopy(renderer, this->text_movement, NULL, &rect_movement_text);

    SDL_Rect rect_movement = {this->options->getScreenWidth() / 2, this->text_start_y, this->options->getShellSize(), this->options->getShellSize()};
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::KEYBOARD_MOVEMENT], NULL, &rect_movement);
    rect_movement.x += this->options->getShellSize();
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::MOUSE_MOVEMENT], NULL, &rect_movement);
    rect_movement.x += this->options->getShellSize();
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::GAMEPAD_MOVEMENT1], NULL, &rect_movement);
    rect_movement.x += this->options->getShellSize();
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::GAMEPAD_MOVEMENT2], NULL, &rect_movement);

    SDL_Rect rect_confirm_text = {this->options->getScreenWidth() / 2, rect_movement.y + rect_movement.h, 0, 0};
    SDL_QueryTexture(this->text_confirm, NULL, NULL, &rect_confirm_text.w, &rect_confirm_text.h);
    rect_confirm_text.x -= rect_confirm_text.w;
    rect_confirm_text.y = rect_confirm_text.y - rect_confirm_text.h / 2 + this->options->getShellSize() / 2;
    SDL_RenderCopy(renderer, this->text_confirm, NULL, &rect_confirm_text);

    SDL_Rect rect_confirm = {this->options->getScreenWidth() / 2, rect_movement.y + rect_movement.h, this->options->getShellSize(), this->options->getShellSize()};
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::KEYBOARD_CONFIRM], NULL, &rect_confirm);
    rect_confirm.x += this->options->getShellSize();
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::MOUSE_CONFIRM], NULL, &rect_confirm);
    rect_confirm.x += this->options->getShellSize();
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::GAMEPAD_CONFIRM], NULL, &rect_confirm);

    SDL_Rect rect_cancel_text = {this->options->getScreenWidth() / 2, rect_confirm.y + rect_confirm.h, 0, 0};
    SDL_QueryTexture(this->text_cancel, NULL, NULL, &rect_cancel_text.w, &rect_cancel_text.h);
    rect_cancel_text.x -= rect_cancel_text.w;
    rect_cancel_text.y = rect_cancel_text.y - rect_cancel_text.h / 2 + this->options->getShellSize() / 2;
    SDL_RenderCopy(renderer, this->text_cancel, NULL, &rect_cancel_text);

    SDL_Rect rect_cancel = {this->options->getScreenWidth() / 2, rect_confirm.y + rect_confirm.h, this->options->getShellSize(), this->options->getShellSize()};
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::KEYBOARD_CANCEL], NULL, &rect_cancel);
    rect_cancel.x += this->options->getShellSize();
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::MOUSE_CANCEL], NULL, &rect_cancel);
    rect_cancel.x += this->options->getShellSize();
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::GAMEPAD_CANCEL], NULL, &rect_cancel);

    SDL_Rect rect_menu_text = {this->options->getScreenWidth() / 2, rect_cancel.y + rect_cancel.h, 0, 0};
    SDL_QueryTexture(this->text_menu, NULL, NULL, &rect_menu_text.w, &rect_menu_text.h);
    rect_menu_text.x -= rect_menu_text.w;
    rect_menu_text.y = rect_menu_text.y - rect_menu_text.h / 2 + this->options->getShellSize() / 2;
    SDL_RenderCopy(renderer, this->text_menu, NULL, &rect_menu_text);

    SDL_Rect rect_menu = {this->options->getScreenWidth() / 2,  rect_cancel.y + rect_cancel.h, this->options->getShellSize(), this->options->getShellSize()};
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::KEYBOARD_MENU], NULL, &rect_menu);
    rect_menu.x += this->options->getShellSize();
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::MOUSE_MENU], NULL, &rect_menu);
    rect_menu.x += this->options->getShellSize();
    SDL_RenderCopy(renderer, this->button_images[(int) ButtonImage::GAMEPAD_MENU], NULL, &rect_menu);
}

void HowToControlsState::loadTexts() {
    if (!this->text_title) {
        this->text_title = fonts->getTexture(renderer, _("Controls"), FontType::TITLE, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});
    }
    if (!this->text_bottom) {
        this->text_bottom = fonts->getTexture(renderer, _("press confirm to continue"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_movement) {
        this->text_movement = fonts->getTexture(renderer, _("movement:"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_confirm) {
        this-> text_confirm = fonts->getTexture(renderer, _("confirm:"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_cancel) {
        this->text_cancel = fonts->getTexture(renderer, _("cancel:"), FontType::NORMAL, {255, 255, 255, 255});
    }
    if (!this->text_menu) {
        this->text_menu = fonts->getTexture(renderer, _("menu:"), FontType::NORMAL, {255, 255, 255, 255});
    }

    this->text_start_y = this->options->getScreenHeight() / 2 - this->options->getShellSize() * 2;
}

void HowToControlsState::loadImages() {
    if (this->button_images.empty()) {
        for (int i = 0; i < (int) ButtonImage::AMOUNT; i++) {
            this->button_images.push_back(NULL);
        }
    }
    for (int i = 0; i < (int) ButtonImage::AMOUNT; i++) {
        switch ((ButtonImage) i)
        {
        case ButtonImage::KEYBOARD_MOVEMENT:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/keyboard_arrows.svg");
            break;
        case ButtonImage::KEYBOARD_CONFIRM:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/keyboard_space.svg");
            break;
        case ButtonImage::KEYBOARD_CANCEL:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/keyboard_backspace.svg");
            break;
        case ButtonImage::KEYBOARD_MENU:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/keyboard_escape.svg");
            break;
        case ButtonImage::MOUSE_MOVEMENT:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/mouse.svg");
            break;
        case ButtonImage::MOUSE_CONFIRM:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/mouse_left.svg");
            break;
        case ButtonImage::MOUSE_CANCEL:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/mouse_right.svg");
            break;
        case ButtonImage::MOUSE_MENU:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/mouse_scroll.svg");
            break;
        case ButtonImage::GAMEPAD_MOVEMENT1:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/switch_stick_l.svg");
            break;
        case ButtonImage::GAMEPAD_MOVEMENT2:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/switch_dpad.svg");
            break;
        case ButtonImage::GAMEPAD_CONFIRM:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/switch_buttons_down.svg");
            break;
        case ButtonImage::GAMEPAD_CANCEL:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/switch_buttons_right.svg");
            break;
        case ButtonImage::GAMEPAD_MENU:
            this->button_images[i] = TextureManager::load(this->renderer, "assets/images/steam_button_start_icon.svg");
            break;
        default:
            break;
        }
    }
}

void HowToControlsState::updateSizing() {
    if (this->text_title) {
        SDL_DestroyTexture(this->text_title);
        this->text_title = NULL;
    }
    if (this->text_bottom) {
        SDL_DestroyTexture(this->text_bottom);
        this->text_bottom = NULL;
    }
    if (this->text_movement) {
        SDL_DestroyTexture(this->text_movement);
        this->text_movement = NULL;
    }
    if (this->text_confirm) {
        SDL_DestroyTexture(this->text_confirm);
        this->text_confirm = NULL;
    }
    if (this->text_cancel) {
        SDL_DestroyTexture(this->text_cancel);
        this->text_cancel = NULL;
    }
    if (this->text_menu) {
        SDL_DestroyTexture(this->text_menu);
        this->text_menu = NULL;
    }
}

State HowToControlsState::getNextState() {
    return this->next_state;
}