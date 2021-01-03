#ifndef AVTrain_h
#define AVTrain_h
#include "Train.h"

class AVTrain : public Train {
    AVTrain(int id_, bool left_,const std::vector<Station*>& stations_);
    ~AVTrain() override ;
    bool isNextMainStation();
};

#endif // AVTrain_h