#include <iostream>
#include <string>
#include <array>
#include <cmath>

static const double gravitational_constant {6.6743e-11};

struct Body {
  int id {};
  double mass {};
  std::array<double, 2> position;
  std::array<double, 2> velocity;
};

double get_distance(const std::array<double, 2>& body_1, const std::array<double, 2>& body_2 ) {
  double x { body_1[0] - body_2[0] };
  double y { body_1[1] - body_2[1] };
  double distance { std::hypot(x, y) };

  return distance;
}


auto main() -> int {

  Body default0 {0, 10, {0.0, 1.0}, {0, 5} };
  Body default1 {0, 10, {1, 0}, {5, 0} };

  double distance { get_distance(default0.position, default1.position) };

  return 0;
}
