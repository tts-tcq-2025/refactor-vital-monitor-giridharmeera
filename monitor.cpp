#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include "./monitor.hpp"

using std::cout, std::flush;
using std::this_thread::sleep_for;
using std::chrono::seconds;

void PrintAlertMessage(const std::string& message) {
    cout << message << "\n";
    for (int i = 0; i < 6; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
}

bool checkVital(const VitalCheck& vital, std::function<void(const std::string&)> alert) {
    if (vital.value < vital.min || vital.value > vital.max) {
        alert(vital.name + " is out of range!");
        return false;
    }
    return true;
}

int vitalsOk(float temperature, float pulseRate, float spo2,
             std::function<void(const std::string&)> alert) {
const VitalCheck vitals[] = {
        {"Temperature", temperature, 95.0, 102.0},
        {"Pulse Rate", pulseRate, 60.0, 100.0},
        {"Oxygen Saturation", spo2, 90.0, 100.0}
    };

    bool allVitalsOk = true;
    for (int i = 0; i < 3; ++i) {
        allVitalsOk = checkVital(vitals[i], alert) && allVitalsOk;
    }

    return allVitalsOk;
}

