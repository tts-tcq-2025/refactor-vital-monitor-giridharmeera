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

static bool isNormalAndNotify(const VitalCheck& v,
                              const std::function<void(const std::string&)>& alert) {
    const VitalBand band = classify(v);
    if (band != VitalBand::NORMAL && alert) {
        ClassifiedVital cv{v, band};
        alert(Message(cv));
    }
    return band == VitalBand::NORMAL;
}

int areAllVitalsNormal(float temperature,
                       float pulseRate,
                       float spo2,
                       std::function<void(const std::string&)> alert,
                       TempUnit tempUnit) {
    const float tempF = normalizeTemperature(temperature, tempUnit);

    const VitalCheck temp   {"Temperature",       tempF,      95.0f, 102.0f};
    const VitalCheck pulse  {"Pulse Rate",        pulseRate,  60.0f, 100.0f};
    const VitalCheck oxygen {"Oxygen Saturation", spo2,       90.0f, 100.0f};

    const VitalCheck vitals[] = {temp, pulse, oxygen};

    bool allNormal = true;
    for (const auto& v : vitals) {
        allNormal = isNormalAndNotify(v, alert) && allNormal;
    }
    return allNormal ? 1 : 0;
}
