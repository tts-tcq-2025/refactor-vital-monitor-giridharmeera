#include "./monitor.h"
#include <assert.h>
#include <thread>
#include <chrono>
#include <iostream>
using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

void AlertMessage(const char* message) {
    cout << message << "\n";
    for (int i = 0; i < 6; i++) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    if (temperature > 102 || temperature < 95) {
        AlertMessage("Temperature is critical!");
        return 0;
    }
    if (pulseRate < 60 || pulseRate > 100) {
        AlertMessage("Pulse Rate is out of range!");
        return 0;
    }
    if (spo2 < 90) {
        AlertMessage("Oxygen Saturation out of range!");
        return 0;
    }
    return 1;
}
