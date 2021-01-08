//Alberto Castagnaro 1219641
#include "SuperAVTrain.h"
#include "Station.h"

SuperAVTrain::SuperAVTrain(int id_, bool left_, const std::vector<Station *> &stations_,const std::vector<int>& timetable_): Train(id_, left_, stations_,300, timetable_) {

}

bool SuperAVTrain::isNextMainStation() {
    return stations[nextStationIndex]->isMain();
}

SuperAVTrain::~SuperAVTrain() =default;
