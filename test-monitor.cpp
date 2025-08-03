#include <gtest/gtest.h>
#include <vector>
#include <string>
#include "./monitor.hpp"

std::vector<std::string> messages;
auto captureAlert = [&messages](const std::string& msg) {
        messages.push_back(msg);
    };

TEST(Monitor, TemperatureOutOfRangeHigh) {
    ASSERT_FALSE(vitalsOk(103.0, 70, 95,captureAlert));
}

TEST(Monitor, TemperatureOutOfRangeLow) {
    ASSERT_FALSE(vitalsOk(94.0, 70, 95,captureAlert));
}

TEST(Monitor, PulseRateOutOfRangeHigh) {
    ASSERT_FALSE(vitalsOk(98.6, 105.0, 95,captureAlert));
}

TEST(Monitor, PulseRateOutOfRangeLow) {
    ASSERT_FALSE(vitalsOk(98.6, 50.0, 95,captureAlert));
}

TEST(Monitor, Spo2OutOfRangeLow) {
    ASSERT_FALSE(vitalsOk(98.6, 70, 85.0,captureAlert));
}

TEST(Monitor, AllVitalsOk) {
    ASSERT_TRUE(vitalsOk(98.6, 70, 95,captureAlert));
}

TEST(Monitor, CustomAlertCapturesMessages) {
    ASSERT_FALSE(vitalsOk(98.6, 105.0, 95, captureAlert));
}







