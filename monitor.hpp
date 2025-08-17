#pragma once
#include <string>
#include <functional>
#include <optional>

// ------------------ Language & Units ------------------
enum class Language { EN, DE };
extern Language g_language; // global selector; default in monitor.cpp

enum class TempUnit { Fahrenheit, Celsius };

// ------------------ Data Model ------------------
struct VitalCheck {
    std::string name;
    float value;     // raw sensor value (temperature may be F or C depending on unit arg)
    float min;       // canonical lower limit in canonical units (F for temp, bpm for pulse, % for SpO2)
    float max;       // canonical upper limit in canonical units
};

// Classification for a single reading after normalization
enum class VitalBand {
    LOW_BREACH,      // below min
    NEAR_LOW,        // within 1.5% of upper-limit from min side (early warning)
    NORMAL,
    NEAR_HIGH,       // within 1.5% of upper-limit to max (early warning)
    HIGH_BREACH      // above max
};

struct ClassifiedVital {
    VitalCheck canonical;   // value converted to canonical units
    VitalBand band;
};

// ------------------ API ------------------

// Core classification helpers (pure functions)
float toFahrenheit(float valueC);                          // C -> F
float normalizeTemperature(float value, TempUnit unit);    // to F

// Compute warning tolerance (1.5% of upper limit), reusable across vitals
inline float warningTolerance(const VitalCheck& v) {
    return 0.015f * v.max;
}

// Map numeric value to VitalBand using spec (min/max and 1.5% of max)
VitalBand classify(const VitalCheck& v);

// Localized user message for a classified vital
std::string messageFor(const ClassifiedVital& cv);

// Compose: check all vitals, call alert() for each warning/breach, and return overall OK?
// Returns 1 if all NORMAL, 0 otherwise (kept original return type semantics).
int areAllVitalsNormal(float temperature,
                       float pulseRate,
                       float spo2,
                       std::function<void(const std::string&)> alert = nullptr,
                       TempUnit tempUnit = TempUnit::Fahrenheit);

// Backward-compatible helper retained from your code (still used internally).
bool checkVital(const VitalCheck& vital,
                std::function<void(const std::string&)> alert);
