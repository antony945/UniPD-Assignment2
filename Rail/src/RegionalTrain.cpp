#include "RegionalTrain.h"

RegionalTrain::RegionalTrain(int id_, bool left_, const std::vector<Station *> &stations_): Train(id_, left_, stations_,160)
{}
RegionalTrain::~RegionalTrain() = default;
