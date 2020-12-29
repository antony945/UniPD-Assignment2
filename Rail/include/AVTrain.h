#ifndef AVTrain_h
#define AVTrain_h
#include "Train.h"

class AVTrain : public Train {
public:
    static const int MAX_SPEED = 240;
public:
    // Costruttore con numero treno, tipo di viaggio e tabella di marcia
    AVTrain(int, int, const std::vector<int>&);
};

#endif // AVTrain_h