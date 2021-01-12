//Alberto Castagnaro 1219641
#ifndef Train_h
#define Train_h
#include <vector>
#include <string>

class Station;

class Train {
public:
    ///distance from station where train is considered out
    static constexpr int STATION_AREA_KM = 5;
    ///Minimum stop time where passengers can get off and get on from the train (minutes)
    static constexpr int MINIMUM_STOP_TIME = 5;
    ///Limited speed near Station(limit begin 5KM before station and 5KM after)
    static constexpr int STATION_MAX_SPEED = 80;
private:
    ///max speed of train (KM/H)
    const int MAX_SPEED;
    ///id to identify the train
    int id;
    ///train direction, true if left, false if right
    bool left;
    ///delay of the train (MINUTES)
    int currentDelay;
    ///current speed of the train (KM/H)
    double currentSpeed;
    ///current distance from first Station (KM)
    double currentDistance;
    ///Index that represent the next Station where the train is going
    int nextStationIndex;
    ///Timetable index
    int timetableIndex;
    ///True if train is in station area (5KM range)
    bool inStation;
    ///time stopped in Station
    int stationStopTime;
    /// Tells if train can transit into station
    bool canTransit;
    /// true = rail has limited speed, false =rail has not
    bool normalRail;
    /// tells if train has ended its run
    bool end;
    /// tells is the train is parked at the moment
    bool parked;
    /*
     * members that help sending signal to stations just one time, avoiding signal sending repetition
     */
    bool firstTimePre20;
    bool firstTime0;
    bool firstTimeArrived;
    ///number that tells in which rail the train actually is (when in range of station)
    int stationRail;

protected:
    ///train timetable
    std::vector<int> timetable;
    ///stations where the train is going to stop
    std::vector<Station*> stations;

    Train(int id_, bool left_,const std::vector<Station*>& stations_, int maxSpeed, const std::vector<int>& timetable_);

public:
    // move and copy constructors/operator= disabled
    Train(const Train&) = delete;
    Train& operator=(const Train&) = delete;
    Train(Train&&) = delete;
    Train& operator=(Train&&) = delete;
    void park();
    void setStationRail(int r);
    int getStationRail() const;
    bool isParked() const;
    std::vector<int> getTimetable() const;
    std::vector<Station*> getStations() const;
    int getNextStationIndex() const;
    int getTimetableIndex() const;
    double getCurrentSpeed() const;
    bool getLeft() const;
    int getCurrentDelay() const;
    int getId() const;
    double getCurrentDistance() const; //KM
    int getMaxSpeed() const;
    bool getEnd() const;
    void setRail(bool);
    bool onNormalRail() const;
    int nextStationTime() const;
    void sendStationRequest();
    void setTransit(bool b);
    bool itCanTransit() const;
    void setStop();
    void setMaxSpeed();
    void setLimitedSpeed();
    bool isInStation() const;
    void enterStation();
    void exitStation();
    bool hasFinish() const;
    void setEnd();
    void setSpeed(double n);
    void increaseDistance();
    bool isWaiting();
    bool justArrived() const;
    void setParking(bool);
    Station& nextStation() const;
    double nextStationDistance() const;
    void setDelay(int currentMinutes);
    bool isEarly(int currentMinutes) const;
    bool hasToStart(int currentMinutes) const;
    virtual bool checkTimetable() = 0;
    virtual bool hasToStop() const = 0;
    virtual std::string getType() const = 0;
    bool firstTimePre20km() const { return firstTimePre20;}
    bool firstTime0km() const { return firstTime0;}
    void setFirstTimePre20km(bool b) { firstTimePre20 = b;}
    void setFirstTime0km(bool b) {firstTime0 = b;}
    void setFirstTimeArrived(bool b) {firstTimeArrived = b;}
    virtual ~Train();
};

#endif // Train_h
