#include "./monitor.h"
#include <gtest/gtest.h>
#include <vector>
#include <string>

TEST(Monitor, TemperatureOutOfRangeHigh) {
    ASSERT_FALSE(vitalsOk(103.0, 70, 95));
}

TEST(Monitor, TemperatureOutOfRangeLow) {
    ASSERT_FALSE(vitalsOk(94.0, 70, 95));
}

TEST(Monitor, PulseRateOutOfRangeHigh) {
    ASSERT_FALSE(vitalsOk(98.6, 105.0, 95));
}

TEST(Monitor, PulseRateOutOfRangeLow) {
    ASSERT_FALSE(vitalsOk(98.6, 50.0, 95));
}

TEST(Monitor, Spo2OutOfRangeLow) {
    ASSERT_FALSE(vitalsOk(98.6, 70, 85.0));
}

TEST(Monitor, AllVitalsOk) {
    ASSERT_TRUE(vitalsOk(98.6, 70, 95));
}

TEST(Monitor, CustomAlertCapturesMessages) {
    std::vector<std::string> messages;
    auto captureAlert = [&messages](const std::string& msg) {
        messages.push_back(msg);
    };
    ASSERT_FALSE(vitalsOk(98.6, 105.0, 95, captureAlert));
    ASSERT_EQ(messages.size(), 1);
    ASSERT_EQ(messages[0], "Pulse Rate is out of range!");
}
