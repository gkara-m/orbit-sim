#include "lib.hpp"

auto main() -> int
{
  auto const lib = library {};

  return lib.name == "orbital_sim" ? 0 : 1;
}
