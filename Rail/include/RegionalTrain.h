//Alberto Castagnaro 1219641
#ifndef RegionalTrain_h
#define RegionalTrain_h
#include "Train.h"

class RegionalTrain : public Train {
public:
    RegionalTrain(int id_, bool left_,const std::vector<Station*>& stations_,const std::vector<int>& timetable_);
    RegionalTrain(const RegionalTrain&)= delete;
    RegionalTrain& operator=(const RegionalTrain&) =delete;
    RegionalTrain(Train&&)=delete;
    RegionalTrain& operator=(RegionalTrain&&)=delete;
    ~RegionalTrain() override ;
};

#endif // RegionalTrain_h