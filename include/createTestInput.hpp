//
// Created by yago on 16/06/29.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Point2D.hpp"
#include <vector>
#include <algorithm>

#ifndef SMARTSTACK_CREATETESTINPUT_H
#define SMARTSTACK_CREATETESTINPUT_H


class createTestInput {


public:
    createTestInput() { }

    void createTestInputFiles(int code, std::string fileName,int n,int p, int min=0, int max=100, double prob=0);

};


#endif //SMARTSTACK_CREATETESTINPUT_H
