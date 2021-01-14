//Alberto Castagnaro 1219641
#include "Train.h"
#include "Station.h"
#include<stdexcept>
#include <utility>
/**
 * Exception thrown if user try to set a speed too high
 */
class SpeedLimitException: std::exception{} ;
/**
 * constructor
 * @param id_ int that represents train ID
 * @param left_ true if train goes left, false if right
 * @param stations_ vector of pointers to station where train is going to transit
 * @param maxSpeed max speed reachable by the train
 * @param timetable_ vector of int representing different times where train will stop to station
 */
Train::Train(int id_, bool left_,const std::vector<Station*>& stations_, int maxSpeed,const std::vector<int>&  timetable_) : id{id_} , left {left_} , stations{stations_}, MAX_SPEED{maxSpeed}, timetable{timetable_} {
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
/**
 *
 * @return vector of int representing stations arrival time of the train
 */
std::vector<int> Train::getTimetable() const {
    return timetable;
}
/**
 *
 * @return true if train is parked, false if it is not
 */
bool Train::isParked() const {
    return parked;
}
/**
 * Function that modified member parked with param p
 * @param p true if train is getting parked, false if not
 */
void Train::setParking(bool p) {
    parked = p;
}
/**
 *
 * @return index of next station of vector stations
 */
int Train::getNextStationIndex() const {
    return nextStationIndex;
}
/**
 *
 * @return index of actual station planned arrival time
 */
int Train::getTimetableIndex() const {
    return timetableIndex;
}
/**
 *
 * @return actual speed
 */
double Train::getCurrentSpeed() const {
    return currentSpeed;
}
/**
 *
 * @return true if travel direction is left, false if right
 */
bool Train::getLeft() const {
    return left;
}
/**
 *
 * @return current delay of the train (it can be negative too)
 */
int Train::getCurrentDelay() const {
    return currentDelay;
}
/**
 *
 * @return int that represents train ID
 */
int Train::getId() const {
    return id;
}
/**
 *
 * @return int of current distance traveled by train from departure station
 */
double Train::getCurrentDistance() const {
    return currentDistance;
}
/**
 * set train speed, if it is higher than max speed throws SpeedLimitException
 * @param n double that represents the speed train should be doing
 */
void Train::setSpeed(double n) {
    if(n>MAX_SPEED){
        throw SpeedLimitException();
    }

    currentSpeed=n;
}
/**
 * increase distance traveled by train per 1 minute
 */
void Train::increaseDistance() {
    currentDistance+=(currentSpeed/60);
}
/**
 *check distance traveled by train from origin and distance of last station from origin
 * @return true if Train has arrived to last station
 */
bool Train::hasFinish() const {
    if(getLeft())
        return currentDistance>=stations[stations.size()-1]->getDistanceLeft();
    else
        return currentDistance>=stations[stations.size()-1]->getDistanceRight();
}
/**
 *
 * @return pointer of the next Station the train has to pass
 */
Station& Train::nextStation() const {
    return *stations[nextStationIndex];
}

/**
 *
 * @return a double of the how far is the next Station from the train
 */
double Train::nextStationDistance() const {
    if(getLeft())
        return currentDistance-stations[nextStationIndex]->getDistanceLeft();
    else
        return currentDistance-stations[nextStationIndex]->getDistanceRight();
}
/**
 * //TODO
 * @return
 */
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

/**
 * send request to next Station for checking if train has the permission to enter into the station
 */
void Train::sendStationRequest() {
    canTransit = nextStation().railRequest(*this);
}
/**
 *
 * @return member transit
 */
bool Train::itCanTransit() const {
    return canTransit;
}
/**
 * modify with b (true or false) if train can transit into the station
 * @param b
 */
void Train::setTransit(bool b) {
    canTransit = b;
}
/**
 * Stop the train setting currentSpeed=0
 */
void Train::setStop() {
    setSpeed(0);
}
/**
 * set Train to its max speed reachable
 */
void Train::setMaxSpeed() {
    setSpeed(MAX_SPEED);
}
/**
 * set Limited speed of 80 KM/H near stations
 */

void Train::setLimitedSpeed() {
    setSpeed(STATION_MAX_SPEED);
}
/**
 *
 * @return true if train is in a station area
 */
bool Train::isInStation() const {
    return inStation;
}

/**
 * set inStation true for let train enter into station
 */
void Train::enterStation() {
    inStation = true;
}
/**
 * get the train out of the station
 */
void Train::exitStation() {
    inStation = false;
    nextStationIndex++;
    if(hasToStop())
        timetableIndex++;

    setFirstTimePre20km(true);
    setFirstTime0km(true);
    setFirstTimeArrived(true);
}
/**
 * @return max_speed
 */
 //Funzione obsoleta dato che nelle classi derivate MAX_SPEED è una public constexpr, implementata per completezza
int Train::getMaxSpeed() const {
    return MAX_SPEED;
}
/**
 *
 * @return true if train has just arrived to station and it can let passengers get on/off
 */
bool Train::justArrived() const {
    return (stationStopTime==0) && firstTimeArrived;
}
/**
 * set delay of the train by param currentMinutes, updated every time the train arrives to a station
 * @param currentMinutes
 */
void Train::setDelay(int currentMinutes) {
    currentDelay = currentMinutes - timetable[timetableIndex];
}
/**
 * check if train has arrived to a station before its schedule time
 * @param currentMinutes int that represents actual time in Railway dailySimulation()
 * @return true if train is early, false if not
 */
bool Train::isEarly(int currentMinutes) const {
    if(hasToStop()) {
        int time_to_reach_station = static_cast<int>(20/(currentSpeed/60));
        return (currentMinutes+time_to_reach_station <= timetable[timetableIndex]);
    } else {
        return false;
    }
}
/**
 * set if train is in a limited speed rail or a limitless speed rail
 * @param r true if rail is a standard rail, false if rail is a transit rail
 */
void Train::setRail(bool r) {
    normalRail = r;
}
/**
 *
 * @return true if train is on a normal rail with limited speed, false if it is not
 */
bool Train::onNormalRail() const {
    return normalRail;
}
/**
 * check with timetable schedule if train has to leave the station
 * @param currentMinutes actual time (int) when function is called
 * @return
 */
bool Train::hasToStart(int currentMinutes) const {
    return currentMinutes>=timetable[timetableIndex];
}
/**
 * park the train setting exact currentDistance 5KM distance from next Station and stopping the train
 */
void Train::park() {
    if(getLeft())
        currentDistance = nextStation().getDistanceLeft()-5;
    else
        currentDistance = nextStation().getDistanceRight()-5;

    setStop();
    parked = true;
    nextStation().depositTrain(*this);
}
/**
 *
 * @return Station arrival time
 */
int Train::nextStationTime() const {
    return timetable[timetableIndex];
}
/**
 *set rail where train can transit
 * @param r
 */
void Train::setStationRail(int r) {
    stationRail = r;
}
/**
 *
 * @return int that represents in which rail train is transiting
 */
int Train::getStationRail() const {
    return stationRail;
}
/**
 * @return vector of pointers to station
 */
std::vector<Station *> Train::getStations() const {
    return stations;
}
/**
 *
 * @return true if train has finished its run
 */
bool Train::getEnd() const {
    return end;
}
/**
 * set end to true , used when train arrives to last station
 */
void Train::setEnd() {
    end = true;
}

Train::~Train() = default;