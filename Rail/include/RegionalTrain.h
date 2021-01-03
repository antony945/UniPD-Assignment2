#ifndef RegionalTrain_h
#define RegionalTrain_h
#include "Train.h"

class RegionalTrain : public Train {
    RegionalTrain(int id_, bool left_,const std::vector<Station*>& stations_);
    ~RegionalTrain() override ;
};

#endif // RegionalTrain_h