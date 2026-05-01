#include "math_utils.hpp"

// ARRAY OPERATOR OVERRIDES
std::array<double, 2> operator+(const std::array<double, 2>& array1, const std::array<double, 2>& array2) {
  double x { array1[0] + array2[0] };
  double y { array1[1] + array2[1] };

  return std::array<double, 2> {x, y};
}
std::array<double, 2> operator+=(std::array<double, 2>& array1, const std::array<double, 2>& array2) {
  array1[0] += array2[0];
  array1[1] += array2[1];

  return array1;
}
std::array<double, 2> operator-(const std::array<double, 2>& array1, const std::array<double, 2>& array2) {
  double x { array1[0] - array2[0] };
  double y { array1[1] - array2[1] };

  return std::array<double, 2> {x, y};
}
std::array<double, 2> operator*(const std::array<double, 2>& array1, const std::array<double, 2>& array2) {
  double x { array1[0] * array2[0] };
  double y { array1[1] * array2[1] };

  return std::array<double, 2> {x, y};
}
std::array<double, 2> operator/(const std::array<double, 2>& array1, const std::array<double, 2>& array2) {
  double x { array1[0] / array2[0] };
  double y { array1[1] / array2[1] };

  return std::array<double, 2> {x, y};
}
std::array<double, 2> operator*(const std::array<double, 2>& array1, const double multiplier) {
  double x { array1[0] * multiplier };
  double y { array1[1] * multiplier };

  return std::array<double, 2> {x,y};
}
std::array<double, 2> operator/(const std::array<double, 2>& array1, const double multiplier) {
  double x { array1[0] / multiplier };
  double y { array1[1] / multiplier };

  return std::array<double, 2> {x,y};
}
