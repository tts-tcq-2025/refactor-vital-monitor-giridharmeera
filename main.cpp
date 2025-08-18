#include <iostream>
#include "monitor.hpp"
#include "monitor/language_selector.hpp"

static void printAlert(const std::string& msg) { std::cout << "[ALERT] " << msg << "\n"; }

int main() {
    setLanguage(Language::EN);
    std::cout << "=== 38.3 C (EN) ===\n";
    std::cout << (areAllVitalsNormal(38.3f, 70.0f, 95.0f, printAlert, TempUnit::Celsius) ? "ALL NORMAL\n" : "WARN/BREACH\n");

    setLanguage(Language::DE);
    std::cout << "=== 38.3 C (DE) ===\n";
    std::cout << (areAllVitalsNormal(38.3f, 70.0f, 95.0f, printAlert, TempUnit::Celsius) ? "ALLES NORMAL\n" : "WARNUNG/ÜBERSCHRITTEN\n");

    setLanguage(Language::EN);
    std::cout << "=== 98.6 F (EN) ===\n";
    std::cout << (areAllVitalsNormal(98.6f, 70.0f, 95.0f, printAlert, TempUnit::Fahrenheit) ? "ALL NORMAL\n" : "WARN/BREACH\n");
    return 0;
}
