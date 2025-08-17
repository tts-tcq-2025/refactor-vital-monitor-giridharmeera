#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include "./monitor.hpp"

using std::cout, std::flush;
using std::this_thread::sleep_for;
using std::chrono::seconds;

// ------------------ Globals ------------------
Language g_language = Language::EN; // default; can be set by tests or main

// ------------------ Utilities ------------------
float toFahrenheit(float valueC) {
    return valueC * 9.0f / 5.0f + 32.0f;
}

float normalizeTemperature(float value, TempUnit unit) {
    return (unit == TempUnit::Celsius) ? toFahrenheit(value) : value;
}

// ------------------ Early-warning Classification ------------------
static inline bool inRange(float x, float lo, float hi) {
    return x >= lo && x <= hi;
}

VitalBand classify(const VitalCheck& v) {
    const float tol = warningTolerance(v); // 1.5% of upper limit
    const float nearLowHi  = v.min + tol;  // [min, min+tol] => NEAR_LOW
    const float nearHighLo = v.max - tol;  // [max-tol, max] => NEAR_HIGH

    if (v.value < v.min)                  return VitalBand::LOW_BREACH;
    if (inRange(v.value, v.min, nearLowHi)) return VitalBand::NEAR_LOW;
    if (inRange(v.value, nearLowHi, nearHighLo)) return VitalBand::NORMAL;
    if (inRange(v.value, nearHighLo, v.max)) return VitalBand::NEAR_HIGH;
    return VitalBand::HIGH_BREACH; // v.value > v.max
}

// ------------------ Localization ------------------
// Key format: "<vital>|<band>"
static std::map<std::string, std::string> EN_MAP = {
    {"Temperature|LOW_BREACH",  "Temperature is out of range! Hypothermia"},
    {"Temperature|NEAR_LOW",    "Warning: Approaching hypothermia"},
    {"Temperature|NORMAL",      "Temperature is normal"},
    {"Temperature|NEAR_HIGH",   "Warning: Approaching hyperthermia"},
    {"Temperature|HIGH_BREACH", "Temperature is out of range! Hyperthermia"},

    {"Pulse Rate|LOW_BREACH",  "Pulse rate is out of range! Bradycardia risk"},
    {"Pulse Rate|NEAR_LOW",    "Warning: Pulse rate near lower limit"},
    {"Pulse Rate|NORMAL",      "Pulse rate is normal"},
    {"Pulse Rate|NEAR_HIGH",   "Warning: Pulse rate near upper limit"},
    {"Pulse Rate|HIGH_BREACH", "Pulse rate is out of range! Tachycardia risk"},

    {"Oxygen Saturation|LOW_BREACH",  "SpO2 is out of range! Hypoxemia"},
    {"Oxygen Saturation|NEAR_LOW",    "Warning: SpO2 near lower limit"},
    {"Oxygen Saturation|NORMAL",      "SpO2 is normal"},
    {"Oxygen Saturation|NEAR_HIGH",   "Warning: SpO2 near upper limit"},
    {"Oxygen Saturation|HIGH_BREACH", "SpO2 reading above configured max"},
};

static std::map<std::string, std::string> DE_MAP = {
    {"Temperature|LOW_BREACH",  "Temperatur außerhalb des Bereichs! Unterkühlung"},
    {"Temperature|NEAR_LOW",    "Warnung: Annäherung an Unterkühlung"},
    {"Temperature|NORMAL",      "Temperatur ist normal"},
    {"Temperature|NEAR_HIGH",   "Warnung: Annäherung an Überhitzung"},
    {"Temperature|HIGH_BREACH", "Temperatur außerhalb des Bereichs! Überhitzung"},

    {"Pulse Rate|LOW_BREACH",  "Puls außerhalb des Bereichs! Bradykardie-Risiko"},
    {"Pulse Rate|NEAR_LOW",    "Warnung: Puls nahe der Untergrenze"},
    {"Pulse Rate|NORMAL",      "Puls ist normal"},
    {"Pulse Rate|NEAR_HIGH",   "Warnung: Puls nahe der Obergrenze"},
    {"Pulse Rate|HIGH_BREACH", "Puls außerhalb des Bereichs! Tachykardie-Risiko"},

    {"Oxygen Saturation|LOW_BREACH",  "SpO2 außerhalb des Bereichs! Hypoxämie"},
    {"Oxygen Saturation|NEAR_LOW",    "Warnung: SpO2 nahe der Untergrenze"},
    {"Oxygen Saturation|NORMAL",      "SpO2 ist normal"},
    {"Oxygen Saturation|NEAR_HIGH",   "Warnung: SpO2 nahe der Obergrenze"},
    {"Oxygen Saturation|HIGH_BREACH", "SpO2-Wert über der konfigurierten Obergrenze"},
};

static const std::map<VitalBand, const char*> BAND_KEY = {
    {VitalBand::LOW_BREACH,  "LOW_BREACH"},
    {VitalBand::NEAR_LOW,    "NEAR_LOW"},
    {VitalBand::NORMAL,      "NORMAL"},
    {VitalBand::NEAR_HIGH,   "NEAR_HIGH"},
    {VitalBand::HIGH_BREACH, "HIGH_BREACH"},
};

static std::string lookupMessage(const std::string& vitalName, VitalBand band) {
    std::string key = vitalName + "|" + BAND_KEY.at(band);
    const auto& langMap = (g_language == Language::DE) ? DE_MAP : EN_MAP;
    auto it = langMap.find(key);
    if (it != langMap.end()) return it->second;
    // fallback English if missing
    auto it2 = EN_MAP.find(key);
    return (it2 != EN_MAP.end()) ? it2->second : (vitalName + " status");
}

std::string messageFor(const ClassifiedVital& cv) {
    return lookupMessage(cv.canonical.name, cv.band);
}

// ------------------ Backward-compatible alert printer ------------------
void PrintAlertMessage(const std::string& message) {
    cout << message << "\n";
    for (int i = 0; i < 6; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
}

// Kept for compatibility; now only checks breach (as original),
// but we’ll use classify(...) for full behavior in areAllVitalsNormal.
bool checkVital(const VitalCheck& vital, std::function<void(const std::string&)> alert) {
    if (vital.value < vital.min || vital.value > vital.max) {
        if (alert) alert(vital.name + " is out of range!");
        return false;
    }
    return true;
}

// ------------------ Orchestrator ------------------
int areAllVitalsNormal(float temperature,
                       float pulseRate,
                       float spo2,
                       std::function<void(const std::string&)> alert,
                       TempUnit tempUnit) {
    // Canonicalize inputs
    float tempF = normalizeTemperature(temperature, tempUnit);

    // Canonical limits (unchanged from your original)
    VitalCheck temp   {"Temperature",       tempF,   95.0f, 102.0f};
    VitalCheck pulse  {"Pulse Rate",        pulseRate,      60.0f, 100.0f};
    VitalCheck oxygen {"Oxygen Saturation", spo2,           90.0f, 100.0f};

    std::vector<VitalCheck> vitals = {temp, pulse, oxygen};

    bool allNormal = true;
    for (const auto& v : vitals) {
        VitalBand band = classify(v);
        ClassifiedVital cv{v, band};

        // Emit messages for warnings and breaches; keep silence on NORMAL
        if (band != VitalBand::NORMAL && alert) {
            alert(messageFor(cv));
        }

        // Overall status: only "all NORMAL" returns 1
        if (band != VitalBand::NORMAL) {
            allNormal = false;
        }
    }
    return allNormal ? 1 : 0;
}
