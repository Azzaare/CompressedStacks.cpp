// Test Run : Implementation

/*==============================================================================
  Includes
==============================================================================*/
#include "include/testrun.hpp"
#include <cstdint>

/*==============================================================================
  Type alias for the different integer size (based on the size of the input)
==============================================================================*/

// using TestRun8 = TestRun<std::uint_least8_t>;
using TestRun16 = TestRun<std::uint_least16_t>;
// using TestRun32 = TestRun<std::uint_least32_t>;
// using TestRun64 = TestRun<std::uint_least64_t>;

/*==============================================================================
  How to use
    * argv[1] is the file name
==============================================================================*/
int main(int argc, char *argv[]) {
  // Getting the path of the instance to test
  std::string filepath = argv[1];

  TestRun16 stack(filepath);
  stack.run();
  stack.println();

  return 0;
}
