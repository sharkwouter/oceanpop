#ifndef OPTIONMANAGER_HPP
#define OPTIONMANAGER_HPP

#include <string>
#include <vector>

#include <json/json.h>

#include "Shell.hpp"

class OptionManager {

private:
    void load();
    void write();

    std::string optionsFile;

    Json::Value options;
    
    bool shell_size_changed = false;

    std::vector<std::vector<ShellType>> loadShells(std::string field);
    void writeShells(std::string field, std::vector<std::vector<ShellType>> shells);
public:
    OptionManager();
    ~OptionManager();

    int getMusicVolume();
    int getSoundVolume();
    bool getChangeMusicOnSwitch();
    bool getStandardModeCompleted();
    int getStandardModeLevel();
    int getChallengeModeHighscore();
    int getChallengeModeLevel();
    int getRelaxedModeScore();
    unsigned int getRelaxedModeSeed();
    std::vector<std::vector<ShellType>> getRelaxedModeShells();
    bool getFullscreen();
    int getScreenWidth();
    int getScreenHeight();
    int getScreenRefreshRate();

    int getShellSize();
    bool getShellSizeChanged();

    void setMusicVolume(int value);
    void setSoundVolume(int value);
    void setChangeMusicOnSwitch(bool value);
    void setStandardModeCompleted(bool value);
    void setStandardModeLevel(int value);
    void setChallengeModeHighscore(int value);
    void setChallengeModeLevel(int value);
    void setRelaxedModeScore(int value);
    void setRelaxedModeSeed(unsigned int value);
    void setRelaxedModeShells(std::vector<std::vector<ShellType>> value);
    void setFullscreen(bool value);
    void setScreenResolution(int width, int height, int refresh_rate);

    void resetChallengeMode();
    void resetStandardMode();
};

#endif // OPTIONMANAGER_HPP