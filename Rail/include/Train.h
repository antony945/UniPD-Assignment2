#ifndef Train_h
#define Train_h
#include <vector>
#include "Station.h"

class Train {
protected:
    //stations where the train is going to stop
    static std::vector<Station*> stations;
    // minimum distance between trains
    static constexpr int MINIMUM_DISTANCE = 10;
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
    // Constructor
    Train(int id_, bool left_,const std::vector<Station*>& stations_, int maxSpeed);
    Train(const Train&)= delete;
    Train& operator==(const Train&) =delete;
    // getter - functions
    double getCurrentSpeed() const;
    int getCurrentDelay() const;
    int getId() const;
    double getCurrentDistance() const;
    bool isStopped() const;
    //tells if train has passed all the stations
    bool hasFinish() const;
    //set speed of train
    void setSpeed(double n);
    //incrementDistance
    void increaseDistance();
    //set if train is arrived to a station
    void setStop();
    //set if train is leaving a station
    void setStart();
    //return next station
    Station* NextStation() const;
    //return Distance from next Station
    int nextStationDistance() const;
    bool isArriving() const;


protected:
    virtual ~Train();
};

#endif // Train_h
