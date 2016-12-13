// ConvexHull : Implementation

/*==============================================================================
  Includes
==============================================================================*/
#include "include/convexHull.hpp"
#include <cstdint>

/*==============================================================================
  Type alias for the different integer size (based on the size of the input)
==============================================================================*/

// using ConvexHull8 = ConvexHull<std::uint_least8_t>;
// using ConvexHull16 = ConvexHull<std::uint_least16_t>;
// using ConvexHull32 = ConvexHull<std::uint_least32_t>;
using ConvexHull64 = ConvexHull<std::uint_least64_t>;

/*==============================================================================
  How to use
    * argv[1] is the file name
==============================================================================*/
int main(int argc, char *argv[]) {
  // Getting the path of the instance to test
  std::string filepath = argv[1];

  ConvexHull64 stack(filepath);
  stack.run();
  //stack.println();

  return 0;
}
