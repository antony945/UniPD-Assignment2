#ifndef SecondaryStation_h
#define SecondaryStation_h
#include "Station.h"

class SecondaryStation : public Station {
    
public:
    SecondaryStation(const std::string& name_, int distance_);
    SecondaryStation();
    bool isMain() override;
    void useTransitRail(Train* myTrain);

    SecondaryStation(const SecondaryStation&) = delete;
    SecondaryStation(SecondaryStation&&) = delete;
    SecondaryStation& operator=(const SecondaryStation&) = delete;
    SecondaryStation& operator=(SecondaryStation&&) = delete;

private:
    std::vector<Rail> transitRails;     //non sono sicuro che possa servire perché questi binari non sono occupabili dato che servono solo al transito, se non serve cancello

};

#endif // SecondaryStation_h