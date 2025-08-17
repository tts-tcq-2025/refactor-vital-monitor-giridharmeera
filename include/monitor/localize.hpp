#pragma once
#include <string>
#include "types.hpp"

std::string messageFor(const ClassifiedVital& cv);

void setLanguage(Language lang);
Language getLanguage();
