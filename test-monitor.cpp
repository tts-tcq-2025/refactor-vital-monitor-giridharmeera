#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "./monitor.hpp"

static std::vector<std::string> messages;
static auto captureAlert = [](const std::string& msg) {
    messages.push_back(msg);
};

static void reset(Language lang = Language::EN) {
    messages.clear();
    g_language = lang;
}

TEST(Monitor, TemperatureOutOfRangeHigh) {
    reset();
    ASSERT_EQ(0, areAllVitalsNormal(103.0f, 70.0f, 95.0f, captureAlert));
    ASSERT_FALSE(messages.empty());
}

TEST(Monitor, TemperatureOutOfRangeLow) {
    reset();
    ASSERT_EQ(0, areAllVitalsNormal(94.0f, 70.0f, 95.0f, captureAlert));
}

TEST(Monitor, PulseRateOutOfRangeHigh) {
    reset();
    ASSERT_EQ(0, areAllVitalsNormal(98.6f, 105.0f, 95.0f, captureAlert));
}

TEST(Monitor, PulseRateOutOfRangeLow) {
    reset();
    ASSERT_EQ(0, areAllVitalsNormal(98.6f, 50.0f, 95.0f, captureAlert));
}

TEST(Monitor, Spo2OutOfRangeLow) {
    reset();
    ASSERT_EQ(0, areAllVitalsNormal(98.6f, 70.0f, 85.0f, captureAlert));
}

TEST(Monitor, AllVitalsOk) {
    reset();
    ASSERT_EQ(1, areAllVitalsNormal(98.6f, 70.0f, 95.0f, captureAlert));
    ASSERT_TRUE(messages.empty()); // no warnings when all normal
}

// ---- Early Warning windows (1.5% of upper limit) ----
// For Temperature: max=102 => tol=1.53
// NEAR_LOW: [95, 96.53]; NEAR_HIGH: [100.47, 102]
TEST(Monitor, TemperatureNearLowWarning) {
    reset();
    ASSERT_EQ(0, areAllVitalsNormal(95.5f, 70.0f, 95.0f, captureAlert));
    ASSERT_FALSE(messages.empty());
    EXPECT_NE(std::string::npos, messages[0].find("Approaching hypothermia"));
}

TEST(Monitor, TemperatureNearHighWarning) {
    reset();
    ASSERT_EQ(0, areAllVitalsNormal(101.0f, 70.0f, 95.0f, captureAlert));
    ASSERT_FALSE(messages.empty());
    EXPECT_NE(std::string::npos, messages[0].find("Approaching hyperthermia"));
}

// ---- Celsius input accepted (converted to F internally) ----
TEST(Monitor, TemperatureCelsiusNormal) {
    reset();
    // 37 C => 98.6 F normal
    ASSERT_EQ(1, areAllVitalsNormal(37.0f, 70.0f, 95.0f, captureAlert, TempUnit::Celsius));
}

TEST(Monitor, TemperatureCelsiusNearHighWarning) {
    reset();
    // ~38.3 C => ~101 F: near-high
    ASSERT_EQ(0, areAllVitalsNormal(38.3f, 70.0f, 95.0f, captureAlert, TempUnit::Celsius));
    ASSERT_FALSE(messages.empty());
}

// ---- German language warnings ----
TEST(Monitor, GermanWarningText) {
    reset(Language::DE);
    ASSERT_EQ(0, areAllVitalsNormal(101.0f, 70.0f, 95.0f, captureAlert));
    ASSERT_FALSE(messages.empty());
    // should contain German wording for near-high temperature
    bool hasGerman = messages[0].find("Warnung") != std::string::npos ||
                     messages[0].find("Überhitzung") != std::string::npos;
    ASSERT_TRUE(hasGerman);
}
