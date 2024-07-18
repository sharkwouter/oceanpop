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

TranslationManager::TranslationManager() : dictionary_manager(tinygettext::DictionaryManager(std::unique_ptr<tinygettext::FileSystem>(new tinygettext::UnixFileSystem))) {
    loadTranslations();
}

TranslationManager::~TranslationManager() {
    
}

void TranslationManager::loadTranslations() {
    std::string system_language = getSystemLanguage();
    std::string language_dir = getResourcePath("assets/languages/");
    
    dictionary_manager.add_directory(language_dir);
    tinygettext::Language language = tinygettext::Language::from_name(system_language);
    if (language) {
        dictionary_manager.set_language(language);
    } else {
        SDL_Log("Couldn't load language %s", system_language.c_str());
    }
}

std::string TranslationManager::getSystemLanguage() {
    return "";
}

std::string TranslationManager::translate(std::string input) {
    return dictionary_manager.get_dictionary().translate(input);
}
