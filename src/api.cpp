#include <iostream>
#include <thread>
#include <chrono>
#include <vector>
#include "monitor/api.hpp"
#include "monitor/units.hpp"
#include "monitor/classify.hpp"
#include "monitor/language_selector.hpp"

using namespace std::chrono;

void PrintAlertMessage(const std::string& message) {
    std::cout << message << "\n";
    for (int i = 0; i < 6; ++i) {
        std::cout << "\r* " << std::flush;
        std::this_thread::sleep_for(seconds(1));
        std::cout << "\r *" << std::flush;
        std::this_thread::sleep_for(seconds(1));
    }
}

bool checkVital(const VitalCheck& vital,
                std::function<void(const std::string&)> alert) {
    if (vital.value < vital.min || vital.value > vital.max) {
        if (alert) alert(vital.name + " is out of range!");
        return false;
    }
    return true;
}

int areAllVitalsNormal(float temperature,
                       float pulseRate,
                       float spo2,
                       std::function<void(const std::string&)> alert,
                       TempUnit tempUnit) {
    // Normalize temp to Fahrenheit (canonical)
    const float tempF = normalizeTemperature(temperature, tempUnit);

    // Canonical limits (same as your original)
    VitalCheck temp   {"Temperature",       tempF,      95.0f, 102.0f};
    VitalCheck pulse  {"Pulse Rate",        pulseRate,  60.0f, 100.0f};
    VitalCheck oxygen {"Oxygen Saturation", spo2,       90.0f, 100.0f};

    std::vector<VitalCheck> vitals = {temp, pulse, oxygen};

    bool allNormal = true;
    for (const auto& v : vitals) {
        VitalBand band = classify(v);
        ClassifiedVital cv{v, band};

        if (band != VitalBand::NORMAL && alert) {
            alert(Message(cv));
        }
        if (band != VitalBand::NORMAL) {
            allNormal = false;
        }
    }
    return allNormal ? 1 : 0;
}
