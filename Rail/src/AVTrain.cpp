//Alberto Castagnaro 1219641
#include "AVTrain.h"

AVTrain::AVTrain(int id_, bool left_, const std::vector<Station *> &stations_,const std::vector<int>& timetable_) : Train(id_, left_, stations_,240, timetable_){

}

AVTrain::~AVTrain() =default;

bool AVTrain::isNextMainStation() {
    return stations[nextStationIndex]->isMain();
}
