#include "SuperAVTrain.h"

SuperAVTrain::SuperAVTrain(int id_, bool left_, const std::vector<Station *> &stations_): Train(id_, left_, stations_,300) {

}

bool SuperAVTrain::isNextMainStation() {
    return stations[nextStationIndex]->isMain();
}

SuperAVTrain::~SuperAVTrain() =default;
