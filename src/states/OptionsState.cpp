#include "OptionsState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "../utils.hpp"
#include "../FontType.hpp"
#include "GameState.hpp"

OptionsState::OptionsState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options, SDL_Window * window) : renderer(renderer), fonts(fonts), sounds(sounds), options(options), window(window),
    theme(renderer, options, Theme::MENU)
{
    loadTexts();
}

OptionsState::~OptionsState() {
    SDL_DestroyTexture(text_title);
    for (int i = 0; i < (int) texts.size(); i++) {
        SDL_DestroyTexture(texts[i]);
        texts[i] = NULL;
    }
}


void OptionsState::handleEvents(std::vector<Event> events) {
    SDL_Point mouse;

    for(Event event :events) {
        switch (event) {
            case Event::UP:
                if (selection != 0) {
                    selection--;
                } else {
                    selection = (int) texts.size() - 1;
                }
                break;
            case Event::DOWN:
                if (selection < ((int) texts.size() - 1)) {
                    selection++;
                } else {
                    selection = 0;
                }
                break;
            case Event::RIGHT:
            case Event::CONFIRM:
                switch ((Option) this->selection) {
                    case Option::CHANGE_MUSIC:
                        changeChangeMusic();
                        break;
                    case Option::SOUND_VOLUME:
                        changeSoundVolume(1);
                        break;
                    case Option::MUSIC_VOLUME:
                        changeMusicVolume(1);
                        break;
#ifdef SHOW_RESOLUTION_OPTIONS
                    case Option::FULLSCREEN:
                        changeFullscreen();
                        break;
                    case Option::RESOLUTION:
                        changeResolution(1);
                        break;
#endif
                    case Option::GO_BACK:
                        this->done = true;
                        break;
                }
                break;
            case Event::LEFT:
                switch ((Option) this->selection) {
                    case Option::CHANGE_MUSIC:
                        changeChangeMusic();
                        break;
                    case Option::SOUND_VOLUME:
                        changeSoundVolume(-1);
                        break;
                    case Option::MUSIC_VOLUME:
                        changeMusicVolume(-1);
                        break;
#ifdef SHOW_RESOLUTION_OPTIONS
                    case Option::FULLSCREEN:
                        changeFullscreen();
                        break;
                    case Option::RESOLUTION:
                        changeResolution(-1);
                        break;
#endif
                    case Option::GO_BACK:
                        this->done = true;
                        break;
                }
                break;
            case Event::QUIT:
                this->next_state = State::EXIT;
                this->done = true;
                break;
            case Event::MENU:
            case Event::CANCEL:
                this->done = true;
                break;
            case Event::MOUSEMOVE:
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if (mouse.y >= this->text_start_y) {
                    int item_size = (int)(((this->options->getScreenHeight() - this->text_start_y) / ((int) texts.size())));
                    this->selection = (int)((mouse.y - this->text_start_y + item_size/4) / item_size);
                    if (this->selection >= (int) texts.size()) {
                        this->selection = (int) texts.size() - 1;
                    }
                }
                break;
            default:
                break;
        }
    }
    
}

void OptionsState::update() {
    this->theme.update();
}

void OptionsState::draw(SDL_Renderer * renderer) {
    this->theme.draw(renderer);

    // Draw title
    SDL_Rect rect_title = {this->options->getScreenWidth() / 2, this->text_start_y / 2, 0, 0};
    SDL_QueryTexture(text_title, NULL, NULL, &rect_title.w, &rect_title.h);
    rect_title.x -= rect_title.w/2;
    rect_title.y -= rect_title.h/2;
    SDL_RenderCopy(renderer, text_title, NULL, &rect_title);

    // Draw options
    for(int i = 0; i < (int) texts.size(); i++) {
        // Draw the option title
        SDL_Rect rect = {this->options->getScreenWidth()/2, getTextY(i), 0, 0};
        SDL_QueryTexture(texts[i], NULL, NULL, &rect.w, &rect.h);
        rect.x -= rect.w/2;

        // Set the texture color
        if(i == selection) {
            // Draw selection box
            SDL_Rect rect_selection = {0, rect.y, this->options->getScreenWidth(), rect.h};
            SDL_SetRenderDrawColor(renderer, COLOR_BOARD.r, COLOR_BOARD.g, COLOR_BOARD.b, COLOR_BOARD.a);
            SDL_RenderFillRect(renderer, &rect_selection);

            // Draw lines around selection box
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawLine(renderer, rect_selection.x, rect_selection.y, rect_selection.x + rect_selection.w, rect_selection.y);
            SDL_RenderDrawLine(renderer, rect_selection.x, rect_selection.y + rect_selection.h, rect_selection.x + rect_selection.w, rect_selection.y + rect_selection.h);
        }

        // Render the option text
        SDL_RenderCopy(renderer, texts[i], NULL, &rect);
    }
}

std::string OptionsState::getSoundVolumeString() {
    std::string result = _("sound volume: ");
    for (int i = 0; i < this->options->getSoundVolume(); i++)
    {
        result += "|";
    }
    
    return result;
}

std::string OptionsState::getMusicVolumeString() {
        std::string result = _("music volume: ");
    for (int i = 0; i < this->options->getMusicVolume(); i++)
    {
        result += "|";
    }
    
    return result;
}

std::string OptionsState::getChangeMusicString() {
    return _("change music upon level switch: ") + std::string(this->options->getChangeMusicOnSwitch() ? _("yes") : _("no"));
}

std::string OptionsState::getFullscreenString() {
    return _("full screen: ") + std::string(this->options->getFullscreen() ? _("yes") : _("no"));
}

std::string OptionsState::getResolutionString() {
    std::string result = _("resolution: ") + std::to_string(this->options->getScreenWidth()) + "x" + std::to_string(this->options->getScreenHeight());
    if (this->options->getScreenRefreshRate() > 0) {
         result += " (" + std::to_string(this->options->getScreenRefreshRate()) + " hz)";
    }
    return result;
}

void OptionsState::changeChangeMusic() {
    this->options->setChangeMusicOnSwitch(!this->options->getChangeMusicOnSwitch());

    SDL_DestroyTexture(this->texts[this->selection]);
    this->texts[this->selection] = fonts->getTexture(renderer, getChangeMusicString(), FontType::NORMAL, {255, 255, 255, 255});
}

void OptionsState::changeSoundVolume(int amount) {
    int max = 4;
    int new_value = this->options->getSoundVolume() + amount;
    if (new_value < 0) {
        new_value = max;
    }
    if (new_value > max) {
        new_value = 0;
    }
    this->options->setSoundVolume(new_value);

    SDL_DestroyTexture(this->texts[this->selection]);
    this->texts[this->selection] = fonts->getTexture(renderer, getSoundVolumeString(), FontType::NORMAL, {255, 255, 255, 255});
}

void OptionsState::changeMusicVolume(int amount) {
    int max = 4;
    int new_value = this->options->getMusicVolume() + amount;
    if (new_value < 0) {
        new_value = max;
    }
    if (new_value > max) {
        new_value = 0;
    }
    this->options->setMusicVolume(new_value);

    SDL_DestroyTexture(this->texts[this->selection]);
    this->texts[this->selection] = fonts->getTexture(renderer, getMusicVolumeString(), FontType::NORMAL, {255, 255, 255, 255});
}

void OptionsState::changeFullscreen() {
    this->options->setFullscreen(!this->options->getFullscreen());

    // Set the refresh rate
    if (options->getFullscreen()) {
        for(SDL_DisplayMode mode : getDisplayModes()) {
            if (mode.w == options->getScreenWidth() && mode.h == options->getScreenHeight() && mode.refresh_rate == options->getScreenRefreshRate()) {
                SDL_SetWindowDisplayMode(window, &mode);
                break;
            }
        }
    }

   SDL_SetWindowFullscreen(this->window, this->options->getFullscreen() ? SDL_WINDOW_FULLSCREEN : 0);

    SDL_DestroyTexture(this->texts[this->selection]);
    this->texts[this->selection] = fonts->getTexture(renderer, getFullscreenString(), FontType::NORMAL, {255, 255, 255, 255});
}

void OptionsState::changeResolution(int amount) {
    std::vector<SDL_DisplayMode> modes = getDisplayModes();
    if (modes.size() > 1) {
        int new_res;
        int current_res = 0;
        for (size_t i = 0; i < modes.size(); i++) {
            if (modes[i].w == this->options->getScreenWidth() && modes[i].h == this->options->getScreenHeight() && modes[i].refresh_rate == this->options->getScreenRefreshRate()) {
                current_res = i;
            }
        }

        new_res = current_res + amount;
        if (new_res < 0) {
            new_res = (((int) modes.size()) - 1);
        } else if (new_res >= ((int) modes.size())) {
            new_res = 0;
        }

        this->options->setScreenResolution(modes[new_res].w, modes[new_res].h, modes[new_res].refresh_rate);
        SDL_SetWindowSize(window, modes[new_res].w, modes[new_res].h);
        if (this->options->getFullscreen()) {
            SDL_SetWindowDisplayMode(this->window, &modes[new_res]);
        }

        updateTexts();
    }
}

void OptionsState::loadTexts() {
    this->text_title = fonts->getTexture(this->renderer, _("Options"), FontType::TITLE, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});
    this->text_start_y = this->options->getScreenHeight() / 4;

    for (int i = 0; i < (((int) Option::GO_BACK) + 1); i++) {
        std::string current_text;
        switch ((Option) i) {
            case Option::SOUND_VOLUME:
                current_text = getSoundVolumeString();
                break;
            case Option::MUSIC_VOLUME:
                current_text = getMusicVolumeString();
                break;
            case Option::CHANGE_MUSIC:
                current_text = getChangeMusicString();
                break;
#ifdef SHOW_RESOLUTION_OPTIONS
            case Option::FULLSCREEN:
                current_text = getFullscreenString();
                break;
            case Option::RESOLUTION:
                current_text = getResolutionString();
                break;
#endif
            case Option::GO_BACK:
                current_text = _("return to menu");
                break;
            default:
                current_text = "?????????";
                break;
        }
        texts.push_back(fonts->getTexture(renderer, current_text, FontType::NORMAL, {255, 255, 255, 255}));
    }
}

void OptionsState::updateTexts() {
    SDL_DestroyTexture(this->text_title);
    for (size_t i = 0; i < this->texts.size(); i++) {
        SDL_DestroyTexture(this->texts[i]);
    }
    this->texts.clear();
    loadTexts();
}

int OptionsState::getTextY(int number) {
    return this->text_start_y + (int)(((this->options->getScreenHeight() - this->text_start_y) / ((int) texts.size()) * number));
}

State OptionsState::getNextState() {
    return this->next_state;
}