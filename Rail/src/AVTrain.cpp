//Alberto Castagnaro 1219641
#include "AVTrain.h"
#include "Station.h"

AVTrain::AVTrain(int id_, bool left_, const std::vector<Station *> &stations_,const std::vector<int>& timetable_) : Train(id_, left_, stations_,AVTrain::MAX_SPEED, timetable_){

}

AVTrain::~AVTrain() =default;

bool AVTrain::hasToStop() const {
    return stations[nextStationIndex]->isMain();
}