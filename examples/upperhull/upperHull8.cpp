// UpperHull : Implementation

/*==============================================================================
  Includes
==============================================================================*/
#include "include/upperHull.hpp"
#include <cstdint>

/*==============================================================================
  Type alias for the different integer size (based on the size of the input)
==============================================================================*/

using UpperHull8 = UpperHull<std::uint_least8_t>;
// using UpperHull16 = UpperHull<std::uint_least16_t>;
// using UpperHull32 = UpperHull<std::uint_least32_t>;
// using UpperHull64 = UpperHull<std::uint_least64_t>;

/*==============================================================================
  How to use
    * argv[1] is the file name
==============================================================================*/
int main(int argc, char *argv[]) {
  // Getting the path of the instance to test
  std::string filepath = argv[1];

  UpperHull8 stack(filepath);
  stack.run();
  // stack.println();

  return 0;
}
