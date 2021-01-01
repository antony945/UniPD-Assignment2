#ifndef AVTrain_h
#define AVTrain_h
#include "Train.h"

class AVTrain : public Train {
public:
    static const int MAX_SPEED = 240;
    int getMaxSpeed() const override { return MAX_SPEED; }
public:
    // Costruttore con numero treno, tipo di viaggio, stazioni percorse e tabella di marcia
    AVTrain(int, bool, const std::vector<Station*>&, const std::vector<int>&);

    void trainInformation(std::ostream& os) const override;
    std::string getType() const override { return "AV TRAIN"; }

    // Dice se treno si deve fermare alla stazione oppure no
    bool sendStopRequest(Station*) override;
};

#endif // AVTrain_h