//Alberto Castagnaro 1219641
#include "AVTrain.h"
#include "Station.h"

/**
 * constructor
 * @param id_ int that represents train ID
 * @param left_ true if train goes left, false if right
 * @param stations_ vector of pointers to station where train is going to transit
 * @param maxSpeed max speed reachable by the train
 * @param timetable_ vector of int representing different times where train will stop to station
 */
AVTrain::AVTrain(int id_, bool left_, const std::vector<Station *> &stations_,const std::vector<int>& timetable_) : Train(id_, left_, stations_,AVTrain::MAX_SPEED, timetable_) {}

AVTrain::~AVTrain() =default;

/**
* check if train has to stop or transit in the next station
* @return true if the next station is main, false if it is secondary
*/
bool AVTrain::hasToStop() const {
    return getStations()[getNextStationIndex()]->isMain();
}

/**
 * check if timetable is compatible with physical features of the train
 * for AV train, timetable considers just main stations
 * @return true if timetable has been edited, false if it has not
 */
bool AVTrain::checkTimetable() {
    std::vector<Station*> mainTrainStations;
    for(Station* s : getStations())
        if(s->isMain()) mainTrainStations.push_back(s);

    bool edited = false;

    for(int i=0; i<mainTrainStations.size()-1; i++) {
        int all_road;
        if(getLeft())
            all_road = mainTrainStations[i+1]->getDistanceLeft()-mainTrainStations[i]->getDistanceLeft();
        else
            all_road = mainTrainStations[i+1]->getDistanceRight()-mainTrainStations[i]->getDistanceRight();

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
                edited = false;
                timetable[i+1] = timetable[i+1]+offset;
            }
        } else {
            timetable.push_back(timetable[i]+min_time+10);
        }
    }

    return edited;
}
