// Test Run : Implementation

/*==============================================================================
  Includes
==============================================================================*/
#include "include/testrunExtras.hpp"
#include <cstdint>

/*==============================================================================
  Type alias for the different integer size (based on the size of the input)
==============================================================================*/

using TestExtras8 = TestExtras<std::uint_least8_t>;
using TestExtras16 = TestExtras<std::uint_least16_t>;
using TestExtras32 = TestExtras<std::uint_least32_t>;
using TestExtras64 = TestExtras<std::uint_least64_t>;

/*==============================================================================
  How to use
    * argv[1] is the file name
    * argv[2] is the code of what is to be done
      * 0 is a convex hull problem with a compressed stack (with extras)
      * 1 is a convex hull problem with a classical stack
      * 2 is a convex hull problem with both stacks comparison (check
errors)
    * argv[3] is the code for the integer size
      * 8 is for 8 bits integer (or smallest size handling 8 bits)
      * 16 is for 16 bits integer (or smallest size handling 16 bits)
      * 32 is for 32 bits integer (or smallest size handling 32 bits)
      * 64 or default is for 64 bits integer (or smallest size handling 64 bits)
==============================================================================*/
int main(int argc, char *argv[]) {
  // Getting the path of the instance to test
  std::string filepath = argv[1];

  bool usecompressed = false;
  bool useclassic = false;

  switch (atoi(argv[2])) {
  case 0: {
    usecompressed = true;
    break;
  }
  case 1: {
    useclassic = true;
    break;
  }
  case 2: {
    usecompressed = true;
    useclassic = true;
    break;
  }
  default:
    throw "At least one type of stack needs to be selected!!!";
  }

  switch (atoi(argv[3])) {
  case 8: {
    TestExtras8 stack(filepath, usecompressed, useclassic);
    if (usecompressed && useclassic) {
      stack.runCompare();
      stack.printCompare();
    } else {
      stack.run();
      stack.println();
    }
    break;
  }
  case 16: {
    TestExtras16 stack(filepath, usecompressed, useclassic);
    if (usecompressed && useclassic) {
      stack.runCompare();
      stack.printCompare();
    } else {
      stack.run();
      stack.println();
    }
    break;
  }
  case 32: {
    TestExtras32 stack(filepath, usecompressed, useclassic);
    if (usecompressed && useclassic) {
      stack.runCompare();
      stack.printCompare();
    } else {
      stack.run();
      stack.println();
    }
    break;
  }
  default:
    TestExtras64 stack(filepath, usecompressed, useclassic);
    if (usecompressed && useclassic) {
      stack.runCompare();
      stack.printCompare();
    } else {
      stack.run();
      stack.println();
    }
  }
  return 0;
}
