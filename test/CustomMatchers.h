#pragma once
#include <gmock/gmock.h>

// https://stackoverflow.com/a/7128822
MATCHER_P(NearWithPrecision, precision, "") { return std::abs(std::get<0>(arg) - std::get<1>(arg)) < precision; }
