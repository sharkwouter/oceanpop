#include "OptionManager.hpp"

#include <SDL.h>

#include <fstream>
#include <iostream>
#include <filesystem>

#include "constants.hpp"
#include "utils.hpp"

OptionManager::OptionManager() {
    // Set path of options file
    this->optionsFile = "";
    char * pref_path = SDL_GetPrefPath(NULL,"oceanpop");
    if (pref_path) {
        this->optionsFile += pref_path;
        SDL_free(pref_path);
    }
    this->optionsFile += "options.json";

    // Load configuration
    load();
}

OptionManager::~OptionManager() {

}

void OptionManager::load() {
    Json::CharReaderBuilder builder;
    std::ifstream optionsStream;
    JSONCPP_STRING errors;
    bool parsing_failed = false;

    optionsStream.open(this->optionsFile, std::ios::in);
    try {
        parseFromStream(builder, optionsStream, &this->options, &errors);

        // Make sure the file can be read
        this->options.get("test", 1).asInt();
    } catch (...) {
        parsing_failed = true;
    }
    optionsStream.close();

    if(parsing_failed) {
        SDL_Log("Couldn't load configuration at %s", this->optionsFile.c_str());
        if (std::filesystem::remove(this->optionsFile)) {
            SDL_Log("Deleted %s", this->optionsFile.c_str());
        } else {
            SDL_Log("Couldn't delete %s", this->optionsFile.c_str());
        }
        this->options = Json::Value();
        return;
    }

    if (!errors.empty()) {
        SDL_Log("Got warning while parsing configuration %s: %s", this->optionsFile.c_str(), errors.c_str());
        return;
    }

    SDL_Log("Configuration loaded at %s", this->optionsFile.c_str());
}

void OptionManager::write() {
    std::ofstream optionsStream(this->optionsFile, std::ios::binary);
    optionsStream << this->options;
    optionsStream.close();
}

std::vector<std::vector<ShellType>> OptionManager::loadShells(std::string field) {
    std::vector<std::vector<ShellType>> result;

    Json::Value array = this->options[field];

    if (array.empty() || !array.isArray() || (int) array.size() == 0 || (int) array[0].size() == 0) {
       return result;
    }

    result.reserve((int) array.size());
    for (int x = 0; x < (int) array.size(); x++) {
        result.push_back(std::move(std::vector<ShellType>()));
        result[x].reserve((int) array[x].size());
        for (int y = 0; y < (int) array[x].size(); y++) {
            result[x].push_back((ShellType) array[x][y].asInt());
        }
    }

    return result;
}

void OptionManager::writeShells(std::string field, std::vector<std::vector<ShellType>> shells) {
    Json::Value array;

    for (int x = 0; x < (int) shells.size(); x++) {
        Json::Value column;
        for (int y = 0; y < (int) shells[x].size(); y++) {
            column.append((int) shells[x][y]);
        }
        array.append(column);
    }
    this->options[field] = array;
    write();
}

int OptionManager::getMusicVolume() {
    return this->options.get("music_volume", 4).asInt();
}

void OptionManager::setMusicVolume(int value) {
    this->options["music_volume"] = value;
    write();
}

int OptionManager::getSoundVolume() {
    return this->options.get("sound_volume", 4).asInt();
}

void OptionManager::setSoundVolume(int value) {
    this->options["sound_volume"] = value;
    write();
}

bool OptionManager::getChangeMusicOnSwitch() {
    return this->options.get("change_music_on_switch", true).asBool();
}

void OptionManager::setChangeMusicOnSwitch(bool value) {
    this->options["change_music_on_switch"] = value;
    write();
}

void OptionManager::resetStandardMode() {
    this->options.removeMember("standard_mode_level");
    write();
}

bool OptionManager::getStandardModeCompleted() {
    return this->options.get("standard_mode_completed", false).asBool();
}

void OptionManager::setStandardModeCompleted(bool value) {
    this->options["standard_mode_completed"] = value;
    write();
}

int OptionManager::getStandardModeLevel() {
    return this->options.get("standard_mode_level", 1).asInt();
}

void OptionManager::setStandardModeLevel(int value) {
    this->options["standard_mode_level"] = value;
    write();
}

void OptionManager::resetChallengeMode() {
    this->options.removeMember("challenge_mode_level");
    this->options.removeMember("challenge_mode_lives");
    this->options.removeMember("challenge_mode_shells");
    write();
}

int OptionManager::getChallengeModeHighscore() {
    return this->options.get("challenge_mode_highscore", 1).asInt();
}

void OptionManager::setChallengeModeHighscore(int value) {
    this->options["challenge_mode_highscore"] = value;
    write();
}

int OptionManager::getChallengeModeLevel() {
    return this->options.get("challenge_mode_level", 1).asInt();
}


void OptionManager::setChallengeModeLevel(int value) {
    this->options["challenge_mode_level"] = value;
    write();
}

int OptionManager::getChallengeModeSeed() {
    return this->options.get("challenge_mode_seed", 0).asInt();
}


void OptionManager::setChallengeModeSeed(int value) {
    this->options["challenge_mode_seed"] = value;
    write();
}

int OptionManager::getRelaxedModeScore() {
    return this->options.get("relaxed_mode_score", 0).asInt();

}
void OptionManager::setRelaxedModeScore(int value) {
    this->options["relaxed_mode_score"] = value;
    write();
}

unsigned int OptionManager::getRelaxedModeSeed() {
    return this->options.get("relaxed_mode_seed", 0).asUInt();

}
void OptionManager::setRelaxedModeSeed(unsigned int value) {
    this->options["relaxed_mode_seed"] = value;
    write();
}

std::vector<std::vector<ShellType>> OptionManager::getRelaxedModeShells() {
    return loadShells("relaxed_mode_shells");
}

void OptionManager::setRelaxedModeShells(std::vector<std::vector<ShellType>> value) {
    writeShells("relaxed_mode_shells", value);
}

bool OptionManager::getFullscreen() {
    return this->options.get("fullscreen", true).asBool();
}

void OptionManager::setFullscreen(bool value) {
    this->options["fullscreen"] = value;
    write();
}

int OptionManager::getScreenWidth() {
    int result = this->options.get("screen_width", 0).asInt();
    if (result == 0) {
        panic("Could not find a valid display mode!");
    }
    return result;
}

int OptionManager::getScreenHeight() {
    int result = this->options.get("screen_height", 0).asInt();
    if (result == 0) {
        panic("Could not find a valid display mode!");
    }
    return result;
}

void OptionManager::setScreenResolution(int width, int height) {
    int new_shell_size = DEFAULT_SHELL_SIZE;
    while (width < (new_shell_size * DEFAULT_BOARD_WIDTH) || height < (new_shell_size * (DEFAULT_BOARD_HEIGHT +1))) {
        new_shell_size = new_shell_size/2;
    }
    // Do not accept smaller shell size than 16
    if (new_shell_size < MIN_SHELL_SIZE) {
        return;
    }

    if (this->options["shell_size"] != new_shell_size) {
        this->options["shell_size"] = new_shell_size;
        this->shell_size_changed = true;
    }

    this->options["screen_width"] = width;
    this->options["screen_height"] = height;

    write();
}

int OptionManager::getShellSize() {
    return this->options.get("shell_size", DEFAULT_SHELL_SIZE).asInt();
}

bool OptionManager::getShellSizeChanged() {
    bool current_shell_size_changed = this->shell_size_changed;
    this->shell_size_changed = false;
    return current_shell_size_changed;
}
