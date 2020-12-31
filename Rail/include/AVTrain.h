#ifndef AVTrain_h
#define AVTrain_h
#include "Train.h"

class AVTrain : public Train {
public:
    static const int MAX_SPEED = 240;
    int getMaxSpeed() const override { return MAX_SPEED; }
public:
    // Costruttore con numero treno, tipo di viaggio e tabella di marcia
    AVTrain(int, bool, const std::vector<int>&);
};

#endif // AVTrain_h