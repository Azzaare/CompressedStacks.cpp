//
// Created by yago on 16/04/19.
//

#ifndef ONEDTRAJECTORIES_ONEDTIMEPOINT_H
#define ONEDTRAJECTORIES_ONEDTIMEPOINT_H


#include "RunningTime.h"

class oneDTimePoint {

double position;
RunningTime t;

public:

    oneDTimePoint(); //Default constructor
    oneDTimePoint(double pos, RunningTime tprima)//parameter Constructor
    {
        position=pos;
        t=tprima;
    }
    oneDTimePoint(double pos, int h,int m,int s) //parameter Constructor 2
    {
        position=pos;
        t=RunningTime(h,m,s);
    }

    oneDTimePoint(const oneDTimePoint &pt); //Copy constructor

    // accessor methods
    double getPos()const{return position;};
    RunningTime getTime()const{return t;};


    void operator=(oneDTimePoint tprima) //assignment
    {
        position=tprima.getPos();
        t=tprima.getTime();
    }

    void write(std::ostream& os)
    {
        os << "running at km point: (" << position << ") at time " << t;
    }
    friend std::ostream& operator<<(std::ostream& os, oneDTimePoint tp ) 
    {
        tp.write(os);
        return os;
    }


};


#endif //ONEDTRAJECTORIES_ONEDTIMEPOINT_H
