#include "AVTrain.h"

AVTrain::AVTrain(int id_, bool left_, const std::vector<Station *> &stations_) : Train(id_, left_, stations_,240){

}

AVTrain::~AVTrain() =default;

bool AVTrain::isNextMainStation() {
    return stations[nextStationIndex]->isMain();
}
