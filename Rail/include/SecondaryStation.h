#ifndef SecondaryStation_h
#define SecondaryStation_h
#include "Station.h"

class SecondaryStation : public Station {
    
public:
    SecondaryStation(const std::string& name_, int distance_);
    SecondaryStation();

private:
    std::vector<Rail> transitRails;     //non sono sicuro che possa servire perché questi binari non sono occupabili dato che servono solo al transito, se non serve cancello
    bool isMain() override;
};

#endif // SecondaryStation_h