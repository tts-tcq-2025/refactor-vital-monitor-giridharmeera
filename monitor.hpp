#pragma once
#include <string>
#include <functional>

struct VitalCheck {
    std::string name;
    float value;
    float min;
    float max;
};

int areAllVitalsNormal(float temperature, float pulseRate, float spo2,
             std::function<void(const std::string&)> alert = nullptr);

