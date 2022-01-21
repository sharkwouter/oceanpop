#include "OptionsState.hpp"

#include "../constants.hpp"
#include "../colors.hpp"
#include "GameState.hpp"

OptionsState::OptionsState(SDL_Renderer * renderer, FontManager * fonts, SoundManager * sounds, OptionManager * options) : renderer(renderer), fonts(fonts), sounds(sounds), options(options),
    theme(renderer, options, Theme::MENU)
{
    this->text_title = fonts->getTexture(renderer, "Options", true, {COLOR_MENU_TITLE.r, COLOR_MENU_TITLE.g, COLOR_MENU_TITLE.b, COLOR_MENU_TITLE.a});

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
            case Option::FULLSCREEN:
                current_text = getFullscreenString();
                break;
            case Option::GO_BACK:
                current_text = "Return to menu";
                break;
            default:
                current_text = "?????????";
                break;
        }
        texts.push_back(fonts->getTexture(renderer, current_text, false, {255, 255, 255, 255}));
    }

    this->text_start_y = getTextY(0);
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
                    case Option::FULLSCREEN:
                        changeFullscreen();
                        break;
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
                    case Option::FULLSCREEN:
                        changeFullscreen();
                        break;
                    case Option::GO_BACK:
                        this->done = true;
                        break;
                }
                break;
            case Event::QUIT:
            case Event::MENU:
            case Event::CANCEL:
                this->done = true;
                break;
            case Event::MOUSEMOVE:
                SDL_GetMouseState(&mouse.x, &mouse.y);
                if (mouse.y >= this->text_start_y) {
                    this->selection = mouse.y/(SCREEN_HEIGHT/((int) this->texts.size() + text_offset)) - text_offset;
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
    SDL_Rect rect_title = {SCREEN_WIDTH / 2, SHELL_SIZE / 2, 0, 0};
    SDL_QueryTexture(text_title, NULL, NULL, &rect_title.w, &rect_title.h);
    rect_title.x -= rect_title.w/2;
    SDL_RenderCopy(renderer, text_title, NULL, &rect_title);

    // Draw options
    for(int i = 0; i < (int) texts.size(); i++) {
        // Draw the option title
        SDL_Rect rect = {SCREEN_WIDTH/2, getTextY(i), 0, 0};
        SDL_QueryTexture(texts[i], NULL, NULL, &rect.w, &rect.h);
        rect.x -= rect.w/2;

        // Set the texture color
        if(i == selection) {
            // Draw selection box
            SDL_Rect rect_selection = {0, rect.y, SCREEN_WIDTH, rect.h};
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
    std::string result = "Sound volume: ";
    for (int i = 0; i < this->options->getSoundVolume(); i++)
    {
        result += "|";
    }
    
    return result;
}

std::string OptionsState::getMusicVolumeString() {
        std::string result = "Music volume: ";
    for (int i = 0; i < this->options->getMusicVolume(); i++)
    {
        result += "|";
    }
    
    return result;
}

std::string OptionsState::getChangeMusicString() {
    return "Change music upon level switch: " + std::string(this->options->getChangeMusicOnSwitch() ? "yes" : "no");
}

std::string OptionsState::getFullscreenString() {
    return "Full screen: " + std::string(this->options->getFullscreen() ? "yes" : "no");
}

void OptionsState::changeChangeMusic() {
    this->options->setChangeMusicOnSwitch(!this->options->getChangeMusicOnSwitch());
    this->texts[this->selection] = fonts->getTexture(renderer, getChangeMusicString(), false, {255, 255, 255, 255});
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
    this->texts[this->selection] = fonts->getTexture(renderer, getSoundVolumeString(), false, {255, 255, 255, 255});
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
    this->texts[this->selection] = fonts->getTexture(renderer, getMusicVolumeString(), false, {255, 255, 255, 255});
}

void OptionsState::changeFullscreen() {
    this->options->setFullscreen(!this->options->getFullscreen());
    this->texts[this->selection] = fonts->getTexture(renderer, getFullscreenString(), false, {255, 255, 255, 255});
}

int OptionsState::getTextY(int number) {
    return SCREEN_HEIGHT/(((int) texts.size())+this->text_offset)*(number+this->text_offset);
}

State OptionsState::getNextState() {
    return State::MENU;
}