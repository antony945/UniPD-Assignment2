#ifndef Station_h
#define Station_h
#include "Train.h"
#include <iostream>

class Station {
public:
    virtual void addTrain(const Train&, int) = 0;
    virtual bool isMain() = 0;
    virtual int getDistance() = 0;
};

#endif // Station_h