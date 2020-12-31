#ifndef SuperAVTrain_h
#define SuperAVTrain_h
#include "Train.h"

class SuperAVTrain : public Train {
public:
    static const int MAX_SPEED = 300;
    int getMaxSpeed() const override { return MAX_SPEED; }
public:
    // Costruttore con numero treno, tipo di viaggio e tabella di marcia
    SuperAVTrain(int, bool, const std::vector<int>&);
};

#endif // SuperAVTrain_h