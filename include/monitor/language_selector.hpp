#pragma once
#include <string>
#include "types.hpp"

// Localized user message for a classified vital
std::string Message(const ClassifiedVital& cv);

void setLanguage(Language lang);
Language getLanguage();
