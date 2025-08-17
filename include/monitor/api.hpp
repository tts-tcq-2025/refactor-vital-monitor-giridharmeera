#pragma once
#include <functional>
#include <string>
#include "types.hpp"

void PrintAlertMessage(const std::string& message);

bool checkVital(const VitalCheck& vital,
                std::function<void(const std::string&)> alert);

int areAllVitalsNormal(float temperature,
                       float pulseRate,
                       float spo2,
                       std::function<void(const std::string&)> alert = nullptr,
                       TempUnit tempUnit = TempUnit::Fahrenheit);
