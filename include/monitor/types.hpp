#pragma once
#include <string>

// ----- Language & Units -----
enum class Language { EN, DE };
enum class TempUnit { Fahrenheit, Celsius };

// Global language selector (as per assignment)
extern Language g_language;

// ----- Data Model -----
struct VitalCheck {
    std::string name;
    float value;  // canonicalized value (°F for temp)
    float min;
    float max;
};

enum class VitalBand {
    LOW_BREACH,
    NEAR_LOW,
    NORMAL,
    NEAR_HIGH,
    HIGH_BREACH
};

struct ClassifiedVital {
    VitalCheck canonical; // value in canonical units
    VitalBand band;
};
