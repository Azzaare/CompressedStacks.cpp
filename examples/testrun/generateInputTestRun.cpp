//
// Created by yago on 16/11/04.
//

#include <fstream>
#include <iostream>
#include <math.h>
#include <string>

using namespace std;

int main(int argc, char *argv[]) {


  string fileName = argv[1];
  int code = atoi(argv[2]);
  int stacktype = atoi(argv[3]);
  std::uint_least64_t n = atoi(argv[4]);
  int p = atoi(argv[5]);
  int min = 0;
  if (argc > 6)
    min = atoi(argv[6]);
  int max = 0;
  if (argc > 7)
    max = atoi(argv[7]);
  double prob = 0;
  if (argc > 8)
    prob = atof(argv[8]);

  // test, 1: CT test ,
  // prob is the
  // probability to pop

   //cout<<":::::::::::::::::::::::::::::::::::::::::::: Going to create file with size "<<n<<" the size of an int happens to be "<<sizeof(int)<<" and  the maximum "<<INT64_MAX<<endl;

  ofstream outfile(fileName.c_str());
  srand(time(NULL));

  // First write the problem parameters
  outfile << "HEADER " << endl;
  outfile << "n " << n << endl;
  if (stacktype == 1) // compressed stack
  {
    outfile << "p " << p << endl;
  }
  outfile << "/HEADER " << endl;


  switch (code) {
  case 1: // push only test
  {
    // now create the actual file
    // pairs of elements (x,0 means push ) (0,1) means pop one (-1,-1) means pop
    // the rest of the stack

    std::uint_least64_t i = 0;
    while (i < n) {
      int contents, action;
      // generate random number
      double randomDraw = (double)rand() / RAND_MAX;

      // cout<<"randomDraw was "<<randomDraw<<endl;
      if (randomDraw > prob) // this element will be pushed
      {
        contents = min + ((randomDraw)) * (max - min);
        action = 0;
      } else {
        contents = 0;
        //  contents = 17; // now I also push myself and then I will pop myself
        action = 1;
      }
      outfile << contents << "," << action << endl;
      i++;
    }
    outfile << -1 << "," << -1 << endl;
    break;
  }
  case 2: { // christmas tree
      // now create the actual file
      // pairs of elements (x,0 means push ) (0,y) means pop y times (-1,-1) means
      // pop the rest of the stack

      std::uint_least64_t i = 1;
      while (i <= n) {
          bool biggerMultiple = false;
          int numPops = 0;
          int number = i;

          std::uint_least64_t biggest = 50;
          std::uint_least64_t power = 1;

          while (biggest * 10 <= i) {
              biggest = biggest * 10;
              power++;
          }

          while (biggest > 50) {
              if (i % biggest == 0) {
                  numPops = numPops + pow(5, (float)power + 1);
                  biggerMultiple = true;
                  number = 0;
                  // cerr<<"HEY!!!!!!!!!!!!!!!!!!! setting  "<<numPops<<" pops for i =
                  // "<<i<<" biggest is "<<biggest<<" and power is "<<power<<endl;
              }
              biggest = biggest / 10;
              power--;
          }

          if (i % 50 == 0 /*&& !biggerMultiple*/) {
              number = 0;
              numPops = numPops + 25;
          }
          outfile << number << "," << numPops << endl;

          i++;
      }
      break;
  }
      case 3: { // tunned christmas tree
          // pairs of elements (x,0 means push ) (0,y) means pop y times (-1,-1) means
          // pop the rest of the stack

          std::uint_least64_t i = 1;
          while (i <= n) {
              bool biggerMultiple = false;
              int numPops = 0;
              int number = i;

              std::uint_least64_t cycle = atoi(argv[6]);
              std::uint_least64_t step = atoi(argv[7]);
              std::uint_least64_t pops = atoi(argv[8]);

              std::uint_least64_t biggest = cycle;
              std::uint_least64_t base = cycle/step;
              int power = 1;

              while (biggest * step <= i) {
                  biggest = biggest * step;
                  power++;
              }

              while (biggest > cycle) {
                  if (i % biggest == 0) {
                      numPops = numPops + pow(pops, (float) power );
                      biggerMultiple = true;
                      number = 0;
                      // cerr<<"HEY!!!!!!!!!!!!!!!!!!! setting  "<<numPops<<" pops for i =
                      // "<<i<<" biggest is "<<biggest<<" and power is "<<power<<endl;
                  }
                  biggest = biggest / step;
                  power--;
              }

              if (i % cycle == 0 /*&& !biggerMultiple*/) {
                  number = 0;
                  numPops = numPops + pops;
              }
              if(number==0) outfile<< i << "," << 0 << endl;
              outfile << number << "," << numPops << endl;
              //if(number==0) cout<< i << "," << 0 << endl;
              //cout<< number << "," << numPops << endl;
              i++;
          }
          break;
  }
  default: // Optional
  {
    cout<<"createTestInput::createTestInputFiles WRONG CODE "<<code<<endl;
    throw(" createTestInput::createTestInputFiles WRONG CODE ");
  }
  }

  outfile.close();

  return 0;
}
