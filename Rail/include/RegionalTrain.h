#ifndef RegionalTrain_h
#define RegionalTrain_h
#include "Train.h"

class RegionalTrain : public Train {
public:
    static const int MAX_SPEED = 160;
    int getMaxSpeed() const override { return MAX_SPEED; }
public:
    // Costruttore con numero treno, tipo di viaggio, stazioni percorse e tabella di marcia
    RegionalTrain(int, bool, const std::vector<Station*>&, const std::vector<int>&);

    void trainInformation(std::ostream& os) const override;
    std::string getType() const override { return "REGIONAL TRAIN"; }

    // Dice se treno si deve fermare alla stazione oppure no
    bool sendStopRequest(Station*) override;
};

#endif // RegionalTrain_h