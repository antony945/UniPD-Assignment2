//Alberto Castagnaro 1219641
#include "RegionalTrain.h"
#include "Station.h"

/**
 * constructor
 * @param id_ int that represents train ID
 * @param left_ true if train goes left, false if right
 * @param stations_ vector of pointers to station where train is going to transit
 * @param maxSpeed max speed reachable by the train
 * @param timetable_ vector of int representing different times where train will stop to station
 */
RegionalTrain::RegionalTrain(int id_, bool left_, const std::vector<Station *> &stations_,const std::vector<int>& timetable_): Train(id_, left_, stations_,RegionalTrain::MAX_SPEED, timetable_) {}

RegionalTrain::~RegionalTrain() = default;

/**
 * function overridden, regional train stops in every station
 * @return true
 */
bool RegionalTrain::hasToStop() const {
    return true;
}

bool RegionalTrain::checkTimetable() {
    bool edited = false;

    for(int i=0; i<stations.size()-1; i++) {
        int all_road;
        if(getLeft())
            all_road = stations[i+1]->getDistanceLeft()-stations[i]->getDistanceLeft();
        else
            all_road = stations[i+1]->getDistanceRight()-stations[i]->getDistanceRight();

        int slow_road;
        int time_to_wait;
        if(i==0) {
            slow_road = STATION_AREA_KM;
            time_to_wait = 0;
        } else {
            slow_road = STATION_AREA_KM*2;
            time_to_wait = MINIMUM_STOP_TIME;
        }
        int fast_road = all_road-slow_road;

        int slow_speed = STATION_MAX_SPEED;
        int fast_speed = MAX_SPEED;

        int slow_time = static_cast<int>(slow_road/(slow_speed/60.0)) + 1;
        int fast_time = static_cast<int>(fast_road/(fast_speed/60.0)) + 1;
        int min_time = static_cast<int>(slow_time+fast_time+time_to_wait);

        if(i<timetable.size()-1) {
            int time_to_do = timetable[i+1]-timetable[i];
            int offset = min_time-time_to_do;

            if(offset > 0) {
                std::cout << "CAMBIATA TIMETABLE\n";
                edited = true;
                timetable[i+1] = timetable[i+1]+offset;
            }
        } else {
            std::cout << "AGGIUNTO VALORE IN TIMETABLE\n";
            timetable.push_back(timetable[i]+min_time+10);
        }
    }
    return edited;
}
