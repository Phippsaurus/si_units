#include <iostream>
#include <string>
#include <vector>

#include "si.h"

using namespace si_units;

int main() {
  auto dist = 21.4_m;
  auto time = 9955.12_s;
  auto speed = dist / time;
  std::cout << "speed: " << speed << '\n';
  std::cout << "area: " << (dist * dist) << '\n';
}
