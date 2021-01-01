#ifndef SuperAVTrain_h
#define SuperAVTrain_h
#include "Train.h"

class SuperAVTrain : public Train {
public:
    static const int MAX_SPEED = 300;
    int getMaxSpeed() const override { return MAX_SPEED; }
public:
    // Costruttore con numero treno, tipo di viaggio, stazioni percorse e tabella di marcia
    SuperAVTrain(int, bool, const std::vector<Station*>&, const std::vector<int>&);

    void trainInformation(std::ostream& os) const override;
    std::string getType() const override { return "SUPER AV TRAIN"; }

    // Dice se treno si deve fermare alla stazione oppure no
    bool sendStopRequest(Station*) override;
};

#endif // SuperAVTrain_h