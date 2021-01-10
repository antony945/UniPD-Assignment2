#ifndef SecondaryStation_h
#define SecondaryStation_h
#include "Station.h"

class SecondaryStation : public Station {
    
public:
    SecondaryStation(const std::string& name_, int distance_);
    SecondaryStation();
    bool isMain() override;
    //metodo che simula l'utilizzo dei binari di transito
    void useTransitRail(Train* myTrain);

    SecondaryStation(const SecondaryStation&) = delete;
    SecondaryStation(SecondaryStation&&) = delete;
    SecondaryStation& operator=(const SecondaryStation&) = delete;
    SecondaryStation& operator=(SecondaryStation&&) = delete;

private:
    std::vector<Rail> transitRails;     
};

#endif // SecondaryStation_h