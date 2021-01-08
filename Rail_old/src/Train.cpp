//Alberto Castagnaro 1219641
#include "Train.h"
#include "Station.h"

Train::Train(int id_, bool left_,const std::vector<Station*>& stations_, int maxSpeed,const std::vector<int>& timetable_) : id{id_} , left {left_} , MAX_SPEED{maxSpeed}, timetable{timetable_} {
    currentDelay=0;
    currentSpeed=0;
    currentDistance=0;
    nextStationIndex=1;
    isInStation=true;
    stationStopTime=0;
    canTransit=false;
    normalRail=true;
    end = false;
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
        setStop();
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

// Tell if train is waiting to make people in
bool Train::isWaiting() {
    if(stationStopTime < 5) {
        stationStopTime++;
        return true;
    }
    else {
        stationStopTime = 0;
        return false;
    }
}

// Get the answer to request at station
void Train::sendStationRequest() {
    // rail request deve tenere conto di t->hasToStop oppure no, se vuole solo
    // transitare o si deve anche fermare
    // deve anche settare se gli sta dando un binario normale, setRail(true)
    // o un binario di transito, setRail(false)
    canTransit = NextStation()->railRequest(this);
}

// return if train canTransit or not
bool Train::itCanTransit() const {
    return canTransit;
}

void Train::setTransit(bool b) {
    canTransit = b;
}

//set if train is arrived to a station
void Train::setStop() {
    setSpeed(0);
}

// Set max speed
void Train::setMaxSpeed() {
    setSpeed(MAX_SPEED);
}

// Set limited speed
void Train::setLimitedSpeed() {
    setSpeed(80);
}

// Has to be true if train is in station area (is setted by outside)
bool Train::isInStation() const {
    return inStation;
}
// Make the train enter station
void Train::enterStation() {
    inStation = true;
}
// Make the train exit station
void Train::exitStation() {
    inStation = false;
    nextStationIndex++;
    if(hasToStop())
        timetableIndex++;
}

bool Train::justArrived() const {
    return stationStopTime==0;
}

void Train::setDelay(int currentMinutes) {
    if(hasToStop()) {
        currentDelay = currentMinutes - timetable[timetableIndex];
    } // altrimenti non devi settare delay, non si sta fermando
}

int Train::nextStationTime() {
    return timetable[nextStationIndex];
}

Train::~Train() = default;


bool Train::hasToStart(int currentMinutes) const {
    return currentMinutes>=timetable[timetableIndex];
}

Train::~Train() = default;
