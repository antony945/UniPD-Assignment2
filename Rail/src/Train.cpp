#include "Train.h"

Train::Train(int id_, bool left_,const std::vector<Station*>& stations_, int maxSpeed) : id{id_} , left {left_} , MAX_SPEED{maxSpeed} {
    currentDelay=0;
    currentSpeed=0;
    currentDistance=0;
    nextStationIndex=1;
    isInStation=true;
    stationStopTime=0;
}

double Train::getCurrentSpeed() const {
    return currentSpeed;
}

bool Train::getLeft() const {
    return left;
}


int Train::getCurrentDelay() const {
    return currentDelay;
}

int Train::getId() const {
    return id;
}

double Train::getCurrentDistance() const {
    return currentDistance;
}

bool Train::isStopped() const {
    return isInStation;
}

void Train::setSpeed(double n) {
    if(n>MAX_SPEED){
        std::cout<<"Speed too high";
        return;
    }
    if(n>80 && (nextStationDistance()<=5000 || (currentDistance-stations[nextStationIndex-1]->getDistance()<5000))){
        std::cout<<"Speed limit of 80 KM/H near stations";
        return;
    }
    currentSpeed=n;
}

void Train::increaseDistance() {
    if(isStopped()) stationStopTime++;
    else if(!isStopped())
        currentDistance+=static_cast<int>(currentSpeed/3.6);
}

void Train::setStop() {
    if(stations[nextStationIndex]->getDistance()==currentDistance && !isStopped()){
        isInStation=true;
    }
}

void Train::setStart() {
    if(stationStopTime==5 && !hasFinish()){
        stationStopTime=0;
        isInStation=false;
        nextStationIndex++;
    }
}

bool Train::hasFinish() const {
    return nextStationIndex==stations.size()+1;
}

Station* Train::NextStation() const {
    return stations[nextStationIndex];
}

int Train::nextStationDistance() const {
    return stations[nextStationIndex]->getDistance()-currentDistance;
}

bool Train::isArriving() const{
    return nextStationDistance()<5000;
}

Train::~Train() = default;


