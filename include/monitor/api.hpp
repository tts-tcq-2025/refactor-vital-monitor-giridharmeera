#pragma once
#include <functional>
#include <string>
#include "types.hpp"

// Legacy printer (kept)
void PrintAlertMessage(const std::string& message);

// Legacy breach-only checker (kept for compatibility)
bool checkVital(const VitalCheck& vital,
                std::function<void(const std::string&)> alert);

// Orchestrator with units & localization support (returns 1 if all NORMAL else 0)
int areAllVitalsNormal(float temperature,
                       float pulseRate,
                       float spo2,
                       std::function<void(const std::string&)> alert = nullptr,
                       TempUnit tempUnit = TempUnit::Fahrenheit);
