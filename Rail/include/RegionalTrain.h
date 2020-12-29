#ifndef RegionalTrain_h
#define RegionalTrain_h
#include "Train.h"

class RegionalTrain : public Train {
public:
    static const int MAX_SPEED = 160;
public:
    // Costruttore con numero treno, tipo di viaggio e tabella di marcia
    RegionalTrain(int, int, const std::vector<int>&);
};

#endif // RegionalTrain_h