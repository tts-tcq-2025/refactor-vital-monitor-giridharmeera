#include "monitor/classify.hpp"

static inline bool isLowBreach(const VitalCheck& v)  { return v.value < v.min; }
static inline bool isHighBreach(const VitalCheck& v) { return v.value > v.max; }

static VitalBand classifyWithinBounds(const VitalCheck& v) {
    const float tol       = warningTolerance(v);    // 1.5% of upper limit
    const float nearLowHi = v.min + tol;            // [min, min+tol] => NEAR_LOW
    const float nearHiLo  = v.max - tol;            // [max-tol, max] => NEAR_HIGH

    if (v.value <= nearLowHi)  return VitalBand::NEAR_LOW;
    if (v.value >= nearHiLo)   return VitalBand::NEAR_HIGH;
    return VitalBand::NORMAL;
}

VitalBand classify(const VitalCheck& v) {
    if (isLowBreach(v))  return VitalBand::LOW_BREACH;
    if (isHighBreach(v)) return VitalBand::HIGH_BREACH;
    return classifyWithinBounds(v);
}
