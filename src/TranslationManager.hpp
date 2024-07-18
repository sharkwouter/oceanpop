#ifndef TRANSLATIONMANAGER_H
#define TRANSLATIONMANAGER_H

#ifdef TRANSLATION_SUPPORT
#include <vector>

#include <tinygettext/tinygettext.hpp>

class TranslationManager {
private:
    tinygettext::DictionaryManager dictionary_manager;
    bool language_set = false;

    std::vector<std::string> getSystemLanguageList();
    void loadTranslations();

public:
    TranslationManager();
    ~TranslationManager();

    std::string translate(std::string input); 
};
#endif

#endif // TRANSLATIONMANAGER_H