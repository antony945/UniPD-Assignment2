#ifndef SuperAVTrain_h
#define SuperAVTrain_h
#include "Train.h"

class SuperAVTrain : public Train {
    SuperAVTrain(int id_, bool left_,const std::vector<Station*>& stations_);
    ~SuperAVTrain() override ;
    bool isNextMainStation();
};

#endif // SuperAVTrain_h