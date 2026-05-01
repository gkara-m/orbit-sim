#pragma once
#include "types.hpp"

#include <array>

std::array<double, 2> operator+(const std::array<double, 2>& array1, const std::array<double, 2>& array2);
std::array<double, 2> operator+=(std::array<double, 2>& array1, const std::array<double, 2>& array2);
std::array<double, 2> operator-(const std::array<double, 2>& array1, const std::array<double, 2>& array2);
std::array<double, 2> operator*(const std::array<double, 2>& array1, const std::array<double, 2>& array2);
std::array<double, 2> operator/(const std::array<double, 2>& array1, const std::array<double, 2>& array2);
std::array<double, 2> operator*(const std::array<double, 2>& array1, const double multiplier);
std::array<double, 2> operator/(const std::array<double, 2>& array1, const double multiplier);
