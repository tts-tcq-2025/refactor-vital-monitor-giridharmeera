#pragma once
#include <string>
#include <functional>

// Struct representing a vital sign check
struct VitalCheck {
    std::string name;
    float value;
    float min;
    float max;
};

// Check one vital sign
bool checkVital(const VitalCheck& vital, std::function<void(const std::string&)> alert);

// Check all vitals
int vitalsOk(float temperature, float pulseRate, float spo2, std::function<void(const std::string&)> alert = nullptr);
