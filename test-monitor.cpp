#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "monitor.hpp"
#include "monitor/language_selector.hpp"

static std::vector<std::string> messages;
static auto captureAlert = [](const std::string& msg) { messages.push_back(msg); };
static void reset(Language lang = Language::EN) { messages.clear(); setLanguage(lang); }

TEST(Monitor, TemperatureCelsiusNormal) {
    reset();
    ASSERT_EQ(1, areAllVitalsNormal(37.0f, 70.0f, 95.0f, captureAlert, TempUnit::Celsius));
}

TEST(Monitor, TemperatureCelsiusNearHighWarning) {
    reset();
    ASSERT_EQ(0, areAllVitalsNormal(38.3f, 70.0f, 95.0f, captureAlert, TempUnit::Celsius));
    ASSERT_FALSE(messages.empty());
}

TEST(Monitor, GermanWarningText) {
    reset(Language::DE);
    ASSERT_EQ(0, areAllVitalsNormal(101.0f, 70.0f, 95.0f, captureAlert));
    ASSERT_FALSE(messages.empty());
    bool hasGerman = messages[0].find("Warnung") != std::string::npos ||
                     messages[0].find("Überhitzung") != std::string::npos;
    ASSERT_TRUE(hasGerman);
}
