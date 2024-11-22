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
        SDL_Log("Checking language %s", system_language.c_str());
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

    SDL_Locale * preferred_locales = SDL_GetPreferredLocales();
    if(preferred_locales) {
        for (SDL_Locale * l = preferred_locales; l->language; l++) {
            std::string language = std::string(l->language);
            std::string country = "";
            if (l->country) {
                country = std::string(l->country);
            }
            std::string char_set = "UTF-8";
            std::string current_locale;
            if (country.empty()) {
                locales.push_back(language + "." + char_set);
            } else {
                locales.push_back(language + "_" + country + "." + char_set);
                locales.push_back(language + "_" + country);
            }
            locales.push_back(language);
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