#include <map>
#include "monitor/language_selector.hpp"

Language g_language = Language::EN; // default

// Key format: "<vital>|<band>"
static std::map<std::string, std::string> EN_MESSAGE = {
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

static std::map<std::string, std::string> DE_MESSAGE = {
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
    const auto& langMap = (g_language == Language::DE) ? DE_MESSAGE : EN_MESSAGE;
    auto it = langMap.find(key);
    if (it != langMap.end()) return it->second;
    auto it2 = EN_MESSAGE.find(key);
    return (it2 != EN_MESSAGE.end()) ? it2->second : (vitalName + " status");
}

std::string Message(const ClassifiedVital& cv) {
    return lookupMessage(cv.canonical.name, cv.band);
}

void setLanguage(Language lang) { g_language = lang; }
Language getLanguage() { return g_language; }
