#include "OptionManager.hpp"

#include <SDL.h>

#include <fstream>
#include <iostream>
#include <filesystem>

#include "utils.hpp"

OptionManager::OptionManager() {
    load();
}

OptionManager::~OptionManager() {

}

void OptionManager::load() {
    Json::CharReaderBuilder builder;
    std::ifstream optionsStream;

    optionsStream.open(getOptionsFilePath(), std::ios::in);
    if (!optionsStream.fail()) {
        parseFromStream(builder, optionsStream, &this->options, NULL);
    } else {
        std::filesystem::create_directories(getOptionsDirPath());
        write();
    }
    optionsStream.close();
}

void OptionManager::write() {
    std::ofstream optionsStream(getOptionsFilePath(), std::ios::binary);
    optionsStream << this->options;
    optionsStream.close();
}

std::string OptionManager::getOptionsFilePath() {
    return getOptionsDirPath() + "options.json";
}

std::string OptionManager::getOptionsDirPath() {
    return SDL_GetPrefPath("match-theory","options");
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

bool OptionManager::getStandardModeCompleted() {
    return this->options.get("standard_mode_completed", true).asBool();
}

void OptionManager::setStandardModeCompleted(bool value) {
    this->options["standard_mode_completed"] = value;
    write();
}

int OptionManager::getStandardModeLevel() {
    return this->options.get("standard_mode_level", 1).asBool();
}

void OptionManager::setStandardModeLevel(int value) {
    this->options["standard_mode_level"] = value;
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

int OptionManager::getChallengeModeLives() {
    return this->options.get("challenge_mode_lives", 3).asInt();
}

void OptionManager::setChallengeModeLives(int value) {
    this->options["challenge_mode_lives"] = value;
    write();
}

int OptionManager::getRelaxedModeScore() {
    return this->options.get("relaxed_mode_score", 0).asInt();

}
void OptionManager::setRelaxedModeScore(int value) {
    this->options["relaxed_mode_score"] = value;
    write();
}

bool OptionManager::getFullscreen() {
    return this->options.get("fullscreen", false).asBool();
}
void OptionManager::setFullscreen(bool value) {
    this->options["fullscreen"] = value;
    write();
}
