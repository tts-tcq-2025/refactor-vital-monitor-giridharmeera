#pragma once
#include "types.hpp"

// Small inline helpers live in the header as they’re tiny.
static inline bool inRange(float x, float lo, float hi) {
    return x >= lo && x <= hi;
}

// 1.5% of upper limit tolerance for early warning
static inline float warningTolerance(const VitalCheck& v) {
    return 0.015f * v.max;
}

VitalBand classify(const VitalCheck& v);
