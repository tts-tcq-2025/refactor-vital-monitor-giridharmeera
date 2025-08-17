#pragma once
#include <string>
#include "types.hpp"

// Localized user message for a classified vital
std::string messageFor(const ClassifiedVital& cv);

// Optional conveniences (you can still set g_language directly)
void setLanguage(Language lang);
Language getLanguage();
