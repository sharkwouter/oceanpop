#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#include <tinygettext/tinygettext.hpp>

class TranslationManager {
private:
    tinygettext::DictionaryManager dictionary_manager;
    bool language_set = false;

    std::string getSystemLanguage();
    void loadTranslations();

public:
    TranslationManager();
    ~TranslationManager();

    std::string translate(std::string input); 
};

#endif // TRANSLATIONMANAGER_H