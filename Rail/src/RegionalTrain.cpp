//Alberto Castagnaro 1219641
#include "RegionalTrain.h"
#include "Station.h"

RegionalTrain::RegionalTrain(int id_, bool left_, const std::vector<Station *> &stations_,const std::vector<int>& timetable_): Train(id_, left_, stations_,RegionalTrain::MAX_SPEED, timetable_) {}

RegionalTrain::~RegionalTrain() = default;

bool RegionalTrain::hasToStop() const {
    return true;
}

bool RegionalTrain::checkTimetable() {
    bool edited = false;

    for(int i=0; i<stations.size()-1; i++) {
        // Prendo distanza tra stazioni
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

        // Prendo tempo di percorrenza segnato
        // Slow speed (TODO: metterla come costante di qualche classe)
        int slow_speed = STATION_MAX_SPEED;
        int fast_speed = MAX_SPEED;
        // output << "FAST SPEED: " << fast_speed << '\n';

        // Tempo minimo per fare la tratta (converti in km/min)
        int slow_time = static_cast<int>(slow_road/(slow_speed/60.0)) + 1;
        int fast_time = static_cast<int>(fast_road/(fast_speed/60.0)) + 1;
        // output << "SLOW_TIME: " << slow_time << '\n';
        // output << "FAST_TIME: " << fast_time << '\n';
        int min_time = static_cast<int>(slow_time+fast_time+time_to_wait);
        // output << "MIN TIME: " << min_time << '\n';
        if(i<timetable.size()-1) {
            int time_to_do = timetable[i+1]-timetable[i];
            int offset = min_time-time_to_do;

            // Se MIN_TIME > TIME_TO_DO DEVO SPOSTARE TUTTO AVANTI DELLA DIFFERENZA TRA QUEI DUE
            if(offset > 0) {
                edited = true;
                timetable[i+1] = timetable[i+1]+offset;
            }
        } else {
            // Se non ci sono abbastanza orari, aggiungi l'orario minimo + 10 minuti
            timetable.push_back(timetable[i]+min_time+10);
        }
    }

    return edited;
}
