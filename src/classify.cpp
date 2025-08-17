#include "monitor/classify.hpp"

VitalBand classify(const VitalCheck& v) {
    const float tol       = warningTolerance(v); // 1.5% of upper limit
    const float nearLowHi = v.min + tol;         // [min, min+tol] => NEAR_LOW
    const float nearHiLo  = v.max - tol;         // [max-tol, max] => NEAR_HIGH

    if (v.value < v.min)                      return VitalBand::LOW_BREACH;
    if (inRange(v.value, v.min, nearLowHi))   return VitalBand::NEAR_LOW;
    if (inRange(v.value, nearLowHi, nearHiLo))return VitalBand::NORMAL;
    if (inRange(v.value, nearHiLo, v.max))    return VitalBand::NEAR_HIGH;
    return VitalBand::HIGH_BREACH;            // v.value > v.max
}
