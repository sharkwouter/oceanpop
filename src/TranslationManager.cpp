#ifdef TRANSLATION_SUPPORT

#include "TranslationManager.hpp"

#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <iostream>

#include <tinygettext/po_parser.hpp>
#include <tinygettext/tinygettext.hpp>
#include <tinygettext/unix_file_system.hpp>

#include "utils.hpp"

TranslationManager::TranslationManager() : dictionary_manager(std::unique_ptr<tinygettext::FileSystem>(new tinygettext::UnixFileSystem)) {
    loadTranslations();
}

TranslationManager::~TranslationManager() {
    
}

void TranslationManager::loadTranslations() {
    std::string language_dir = getResourcePath("assets/languages/");
    dictionary_manager.add_directory(language_dir);

    
    for(std::string system_language : getSystemLanguageList()) {
        tinygettext::Language language = tinygettext::Language::from_name(system_language);
        if (language) {
            for (tinygettext::Language l: dictionary_manager.get_languages()) {
                if (l == language) {
                    dictionary_manager.set_language(language);
                    SDL_Log("Using language %s", language.get_language().c_str());
                    language_set = true;
                    return;
                }
            }
        }
    }
    SDL_Log("Using default language English");;
}

std::vector<std::string> TranslationManager::getSystemLanguageList() {
    std::vector<std::string> locales;

    #include <locale.h>
    char * locale_c_str = setlocale(LC_ALL, "");
    if (locale_c_str != NULL){
        std::string locale(locale_c_str);
        free(locale_c_str);

        locales.push_back(locale);
        if (locale.find(".") != std::string::npos) {
            locales.push_back(locale.substr(0, locale.find(".")));
        }
        if (locale.find("_") != std::string::npos) {
            locales.push_back(locale.substr(0, locale.find("_")));
        }
    }

    return locales;
}

std::string TranslationManager::translate(std::string input) {
    if (language_set) {
        return dictionary_manager.get_dictionary().translate(input);
    } else {
        return input;
    }
}

#endif // TRANSLATION_SUPPORT