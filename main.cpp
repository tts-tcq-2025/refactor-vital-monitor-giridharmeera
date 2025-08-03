#include <iostream>
#include <thread>
#include <string>
#include <chrono>
#include "./monitor.hpp"

void PrintAlertMessage(const std::string& message) {
    cout << message << "\n";
    for (int i = 0; i < 6; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
}

int main() {
    vitalsOk(103.0, 70, 95,PrintAlertMessage);
}
