#ifndef RegionalTrain_h
#define RegionalTrain_h
#include "Train.h"

class RegionalTrain : public Train {
public:
    static const int MAX_SPEED = 160;
    int getMaxSpeed() const override { return MAX_SPEED; }
public:
    // Costruttore con numero treno, tipo di viaggio e tabella di marcia
    RegionalTrain(int, bool, const std::vector<int>&);
};

#endif // RegionalTrain_h