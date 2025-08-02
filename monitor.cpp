#include "./monitor.h"
#include <assert.h>
#include <thread>
#include <chrono>
#include <iostream>
using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

// Default alert: console output with blinking stars
void defaultAlert(const std::string& message) 
{
    cout << message << "\n";
    for (int i = 0; i < 6; ++i) 
    {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
}

// Check one vital
bool checkVital(const VitalCheck& vital, std::function<void(const std::string&)> alert) 
{
    if (vital.value < vital.min || vital.value > vital.max) 
    {
        alert(vital.name + " is out of range!");
        return false;
    }
    return true;
}

// Main function to check all vitals 
int vitalsOk(float temperature, float pulseRate, float spo2, std::function<void(const std::string&)> alert) 
{
    if (!alert) 
    {
        alert = defaultAlert;
    }

    VitalCheck vitals[] = 
    {
        {"Temperature", temperature, 95.0, 102.0},
        {"Pulse Rate", pulseRate, 60.0, 100.0},
        {"Oxygen Saturation", spo2, 90.0, 100.0}
    };

    for (int i = 0; i < 3; ++i) 
    {
        if (!checkVital(vitals[i], alert)) 
        {
            return 0;
        }
    }

    return 1;
}
