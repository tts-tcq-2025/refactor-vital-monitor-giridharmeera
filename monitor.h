#pragma once
#include <string>
#include <functional>

struct VitalCheck {
    std::string name;
    float value;
    float min;
    float max;
};

bool checkVital(const VitalCheck& vital, std::function<void(const std::string&)> alert);

int vitalsOk(float temperature, float pulseRate, float spo2,
             std::function<void(const std::string&)> alert = nullptr);
