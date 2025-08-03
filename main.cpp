#include <iostream>
#include <thread>
#include <string>
#include <chrono>
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

int main() {
    vitalsOk(103.0, 70, 95,PrintAlertMessage);
}
