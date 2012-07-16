#include "safetymargin.hpp"

double SafetyMargin::sm = 0.15;

SafetyMargin::SafetyMargin() {
}

double SafetyMargin::makeSM(double _value) {
  return _value + _value*sm;
}

