//Alberto Castagnaro 1219641
#include "Train.h"
#include "Station.h"
#include<stdexcept>
#include <utility>

class SpeedLimitException: std::exception{} ;

Train::Train(int id_, bool left_,const std::vector<Station*>& stations_, int maxSpeed,const std::vector<int>&  timetable_) : id{id_} , left {left_} , MAX_SPEED{maxSpeed}, timetable{timetable_} {
    currentDelay=0;
    currentSpeed=0;
    currentDistance=0;
    nextStationIndex=0;
    timetableIndex=0;
    inStation=true;
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

// TODO: Far fare ad alberto le eccezioni e non il cout
void Train::setSpeed(double n) {
    if(n>MAX_SPEED){
        throw SpeedLimitException();
    }
    if(n>80 && (nextStationDistance()<=5000 || (currentDistance-stations[nextStationIndex-1]->getDistance()<5000))){
        std::cout<<"Speed limit of 80 KM/H near stations";
        return;
    }
    currentSpeed=n;
}

void Train::increaseDistance() {
    currentDistance+=static_cast<int>(currentSpeed/60);
}

bool Train::hasFinish() const {
    return currentDistance>=stations[stations.size()-1]->getDistance();
}

Station* Train::NextStation() const {
    return stations[nextStationIndex];
}

// TODO: Da modificare a seconda di left o meno
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

    // se è tanto in anticipo metti canTransit a false e metti/lascia il treno in parcheggio
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

bool Train::inAnticipo(int currentMinutes) const {
    if(hasToStop()) {
        // se si deve fermare, controlla se il currentMinutes è molto minore (almeno 20 minuti) dell'orario indicato nella timetable
        // considerando che deve fare ancora 20 km
        int time_to_reach_station = 20/(currentSpeed/60);
        return (currentMinutes+time_to_reach_station+20 <= timetable[timetableIndex]);
    } else {
        // Se non si deve fermare sicuramente non dovrà fermarsi in parcheggio a causa dell'anticipo-
        return false;
    }
}

void Train::setRail(bool r) {
    normalRail = r;
}

bool Train::onNormalRail() const {
    return normalRail;
}

bool Train::hasToStart(int currentMinutes) const {
    return currentMinutes>=timetable[timetableIndex];
}

Train::~Train() = default;