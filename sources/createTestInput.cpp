//
// Created by yago on 16/06/29.
//

#include "createTestInput.h"

using namespace std;

void createTestInput::createTestInputFiles(int code, string fileName,int n,int p, int min, int max, double prob ) //code: 0 pushOnly test, 1: CT test , prob is the probability to pop
{

    string line;
    ofstream outfile (fileName.c_str());
    srand(time(NULL));

    switch(code){
        case 0  : // push only test
        {
            // First write the problem parameters
            outfile << n << "," << p << endl;
            // now create the actual file
            // pairs of elements (x,0 means push ) (0,1) means pop one (-1,-1) means pop the rest of the stack

            int i = 0;
            while (i < n)
            {
                int contents, action;
                // generate random number
                double randomDraw = (double)rand() / RAND_MAX;

                //cout<<"randomDraw was "<<randomDraw<<endl;
                if (randomDraw > prob)// this element will be pushed
                {
                    contents = min + ((randomDraw)) * (max - min);
                    action = 0;
                }
                else {
                    contents = 0;
                    action = 1;
                }
                outfile << contents << "," << action << endl;
                i++;
            }
            outfile << -1 << "," << -1 << endl;
            break; //optional
        }
        case 1 :
        {
            // First write the problem parameters
            outfile << n << "," << p << endl;

            // now create the actual file
            // pairs of elements (x,0 means push ) (0,y) means pop y times (-1,-1) means pop the rest of the stack

            int i = 1;
            while (i <= n)
            {
               bool biggerMultiple=false;
               int numPops=0;
               int number=i;

                int biggest=50;

                while(biggest*10<=i) {
                    biggest = biggest * 10;
                }

                while(biggest>50&&!biggerMultiple) {
                    if (i % biggest == 0) {
                        numPops = biggest * 0.45;
                        biggerMultiple = true;
                        number=0;
                    }
                    else {
                        biggest = biggest / 10;
                    }
                }
                if (i%50==0 && !biggerMultiple ) {
                    number=0;
                    numPops = 25;
                }
                outfile << number << "," << numPops << endl;
                i++;
            }


            break; //optional
        }
        default : //Optional
        {throw (" createTestInput::createTestInputFiles WRONG CODE ");}
    }

    outfile.close();





}
