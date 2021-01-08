//Alberto Castagnaro 1219641
#include "RegionalTrain.h"

RegionalTrain::RegionalTrain(int id_, bool left_, const std::vector<Station *> &stations_,const std::vector<int>& timetable_): Train(id_, left_, stations_,RegionalTrain::MAX_SPEED, timetable_)
{}
RegionalTrain::~RegionalTrain() = default;

bool RegionalTrain::hasToStop() const {
    return true;
}