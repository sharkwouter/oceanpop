#ifndef OPTIONMANAGER_HPP
#define OPTIONMANAGER_HPP

#include <string>

#include <json/json.h>

class OptionManager {

private:
    void load();
    void write();

    std::string getOptionsDirPath();
    std::string getOptionsFilePath();

    Json::Value options;
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
    int getChallengeModeLives();
    int getRelaxedModeScore();
    bool getFullscreen();

    void setMusicVolume(int value);
    void setSoundVolume(int value);
    void setChangeMusicOnSwitch(bool value);
    void setStandardModeCompleted(bool value);
    void setStandardModeLevel(int value);
    void setChallengeModeHighscore(int value);
    void setChallengeModeLevel(int value);
    void setChallengeModeLives(int value);
    void setRelaxedModeScore(int value);
    void setFullscreen(bool value);
};

#endif // OPTIONMANAGER_HPP