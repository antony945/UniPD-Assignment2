//Alberto Castagnaro 1219641
#ifndef Train_h
#define Train_h
#include <vector>
#include "Station.h"

class Train {
protected:
    //stations where the train is going to stop
    std::vector<Station*> stations;
    // minimum distance between trains
    static constexpr int MINIMUM_DISTANCE = 10;
    //train timetable
    std::vector<int> timetable;
    //max speed of train (KM/H)
    const int MAX_SPEED;
    //delay of the train (MINUTES)
    int currentDelay;
    //id to identify the train
    int id;
    //train direction, true if left, false if right
    bool left;
    //current speed of the train (KM/H)
    double currentSpeed;
    //current distance from first Station (M)
    int currentDistance;
    //Index that represent the next Station where the train is going
    int nextStationIndex;
    //True if train is in a Station
    bool isInStation;
    //time stopped in Station
    int stationStopTime;
public:
    // Constructors
    Train(int id_, bool left_,const std::vector<Station*>& stations_, int maxSpeed,const std::vector<int>& timetable_);
    Train(const Train&)= delete;
    Train& operator=(const Train&) =delete;
    Train(Train&&)=delete;
    Train& operator=(Train&&)=delete;
    // getter - functions
    double getCurrentSpeed() const;
    bool getLeft() const;
    int getCurrentDelay() const;
    int getId() const;
    double getCurrentDistance() const; //KM
    bool isStopped() const;
    //tells if train has passed all the stations
    bool hasFinish() const;
    //set speed of train
    void setSpeed(double n);
    //increment Distance per minute
    void increaseDistance();
    //set if train is arrived to a station
    void setStop();
    //set if train is leaving a station
    void setStart();
    //return next station
    Station* NextStation() const;
    //return Distance from next Station
    int nextStationDistance() const;
    //train in range of 20 km
    bool isArriving() const;
    //next Station Time
    int nextStationTime();

public:
    virtual ~Train();
};

#endif // Train_h
