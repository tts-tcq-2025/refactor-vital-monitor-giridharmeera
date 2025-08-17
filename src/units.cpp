#include "monitor/units.hpp"

float toFahrenheit(float valueC) {
    return valueC * 9.0f / 5.0f + 32.0f;
}

float normalizeTemperature(float value, TempUnit unit) {
    return (unit == TempUnit::Celsius) ? toFahrenheit(value) : value;
}
