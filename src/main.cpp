// Main file of the compressed stack library
#include "../include/sign.hpp"
#include "../include/data.hpp"
#include "../include/normalStack.hpp"

// Test Signature
void testSign()
{
  Signature<int> sign (1, 0, 1);
  sign.println();
}

// Test data
void testData()
{
  Data<int> d (1,2);
  d.println();

  Data<int> d1 (1,2);
  Data<int> d2 (2,3);
  Data<int> d3 (3,5);
  Data<int> d4 (4,12);
  Data<int> d5 (5,892);

  Explicit<int> xplicit = initExplicit<int>();
  xplicit.push_back(d1);
  xplicit.back();
  xplicit.pop_back();
}

// Test normal stack
void testNormalStack()
{
  NormalStack<int> stack (5);
  Data<int> d1 (1,2);
  Data<int> d2 (2,3);
  Data<int> d3 (3,5);
  Data<int> d4 (4,12);
  Data<int> d5 (5,892);
  stack.push(d1);
  stack.push(d2);
  stack.push(d3);
  stack.push(d4);
  stack.push(d5);
  stack.println();
}

// Main
int main(int argc, char const *argv[]) {
  testSign();
  testData();
  testNormalStack();
  return 0;
}
