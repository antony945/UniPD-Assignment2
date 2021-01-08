//Alberto Castagnaro 1219641
#include "SuperAVTrain.h"
#include "Station.h"

SuperAVTrain::SuperAVTrain(int id_, bool left_, const std::vector<Station *> &stations_,const std::vector<int>& timetable_): Train(id_, left_, stations_,SuperAVTrain::MAX_SPEED, timetable_) {

}

bool SuperAVTrain::hasToStop() const {
    return stations[nextStationIndex]->isMain();
}

SuperAVTrain::~SuperAVTrain() =default;
