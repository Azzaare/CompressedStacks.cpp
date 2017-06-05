//
// Created by yago on 16/11/04.
//

#include "include/point2D.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
#include <vector>

using namespace std;

int main(int argc, char *argv[]) {

  string fileName = argv[1];
  int stacktype = atoi(argv[2]);
  std::uint_least64_t n = atoi(argv[3]);
  int p = atoi(argv[4]);
  int min = 0;
  if (argc > 5)
    min = atoi(argv[5]);
  int max = 0;
  if (argc > 6)
    max = atoi(argv[6]);

  ofstream outfile(fileName.c_str());
  srand(time(NULL));

  // First write the problem parameters
  outfile << "HEADER " << endl;
  outfile << "n " << n << endl;
  if (stacktype == 1) // compressed stack
  {
    outfile << "p " << p << endl;
    outfile << "b "
            << "2" << endl; // buffer for the CH problem
  }
  outfile << "/HEADER " << endl;

  std::vector<Point2D> pointsToSort = vector<Point2D>();

  std::uint_least64_t i = 0;
  while (i < n) {

    // create output for the upper hull problem.
    // in this case, max and min stand for the maximum and minimum values of x
    // and y
    // generate a random point in the (min,max)2 range

    double randomx = (double)rand() / RAND_MAX;
    randomx = min + (max - min) * randomx;
    double randomy = (double)rand() / RAND_MAX;
    randomy = min + (max - min) * randomy;

    //      cout<<"generated point "<<randomx<<" "<<randomy<<"
    //      "<<pointsToSort.size()<<endl;

    pointsToSort.push_back(Point2D(randomx, randomy));
    i++;
  }

  // sort the vector
  std::sort(pointsToSort.begin(), pointsToSort.end());

  // add first the point (min,min)
  outfile << min << "," << min << endl;

  // add the sorted points
  for (int j = 0; j < (int)pointsToSort.size(); j++) {
    outfile << pointsToSort[j].GetX() << "," << pointsToSort[j].GetY() << endl;
  }

  // add finally (max,min)
  outfile << max << "," << min << endl;

  outfile.close();

  return 0;
}
