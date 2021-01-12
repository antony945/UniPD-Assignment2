//Alberto Castagnaro 1219641
#include "Train.h"
#include "Station.h"
#include<stdexcept>
#include <utility>

class SpeedLimitException: std::exception{} ;

Train::Train(int id_, bool left_,const std::vector<Station*>& stations_, int maxSpeed,const std::vector<int>&  timetable_) : id{id_} , left {left_} , stations{stations_}, MAX_SPEED{maxSpeed}, timetable{std::move(timetable_)} {    
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
    parked = false;
    firstTimePre20 = true;
    firstTime0 = true;
    firstTimeArrived = true;
    stationRail = -1;
}

std::vector<int> Train::getTimetable() const {
    return timetable;
}

bool Train::isParked() const {
    return parked;
}

void Train::setParking(bool p) {
    parked = p;
}

int Train::getNextStationIndex() const {
    return nextStationIndex;
}

int Train::getTimetableIndex() const {
    return timetableIndex;
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

void Train::setSpeed(double n) {
    if(n>MAX_SPEED){
        throw SpeedLimitException();
    }

    currentSpeed=n;
}

void Train::increaseDistance() {
    currentDistance+=(currentSpeed/60);
}

bool Train::hasFinish() const {
    if(left)
        return currentDistance>=stations[stations.size()-1]->getDistanceLeft();
    else
        return currentDistance>=stations[stations.size()-1]->getDistanceRight();
}

Station& Train::nextStation() const {
    return *stations[nextStationIndex];
}

// TODO: Da modificare a seconda di left o meno
double Train::nextStationDistance() const {
    if(left)
        return currentDistance-stations[nextStationIndex]->getDistanceLeft();
    else
        return currentDistance-stations[nextStationIndex]->getDistanceRight();
}

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

// TODO: DA CONTROLLARE
void Train::sendStationRequest() {
    // rail request deve tenere conto di t->hasToStop oppure no, se vuole solo
    // transitare o si deve anche fermare
    // deve anche settare se gli sta dando un binario normale, setRail(true)
    // o un binario di transito, setRail(false)
    canTransit = nextStation().railRequest(*this);
}

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
// TODO: DA CONTROLLARE
void Train::exitStation() {
    inStation = false;
    nextStationIndex++;
    if(hasToStop())
        timetableIndex++;

    setFirstTimePre20km(true);
    setFirstTime0km(true);
    setFirstTimeArrived(true);
}

int Train::getMaxSpeed() const {
    return MAX_SPEED;
}

bool Train::justArrived() const {
    return (stationStopTime==0) && firstTimeArrived;
}

void Train::setDelay(int currentMinutes) {
    currentDelay = currentMinutes - timetable[timetableIndex];
}

// TODO: DA CONTROLLARE
bool Train::inAnticipo(int currentMinutes) const {
    if(hasToStop()) {
        // se si deve fermare, controlla se andando alla minore velocità possibile
        // considerando che deve fare ancora 20 km
        // raggiungerebbe la stazione prima dell'orario indicato dalla timetable
        int time_to_reach_station = 20/(160/60);
        return (currentMinutes+time_to_reach_station <= timetable[timetableIndex]);
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

void Train::park() {
    if(left)
        currentDistance = nextStation().getDistanceLeft()-5;
    else
        currentDistance = nextStation().getDistanceRight()-5;

    setStop();
    parked = true;
    nextStation().depositTrain(*this);
}

Train::~Train() = default;