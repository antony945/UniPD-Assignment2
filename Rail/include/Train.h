//Alberto Castagnaro 1219641
#ifndef Train_h
#define Train_h
#include <vector>

class Station;

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
    //id to identify the train
    int id;
    //train direction, true if left, false if right
    bool left;
    //delay of the train (MINUTES)
    int currentDelay;
    //current speed of the train (KM/H)
    double currentSpeed;
    //current distance from first Station (KM)
    int currentDistance;
    //Index that represent the next Station where the train is going
    int nextStationIndex;
    //Timtable index
    int timetableIndex;
    //True if train is in a Station
    bool inStation;
    //time stopped in Station
    int stationStopTime;
    //tell train if it can transitate in station or has to park
    bool canTransit;
    //tell which rail it will use for the station (true = normalRail, false = transitRail)
    bool normalRail;
public:
    // Constructors
    Train(int id_, bool left_,const std::vector<Station*>& stations_, int maxSpeed, const std::vector<int>& timetable_);
    Train(const Train&) = delete;
    Train& operator=(const Train&) = delete;
    Train(Train&&) = delete;
    Train& operator=(Train&&) = delete;
    
    // getter - functions
    double getCurrentSpeed() const;
    bool getLeft() const;
    int getCurrentDelay() const;
    int getId() const;
    double getCurrentDistance() const; //KM
    // set normal rail or transit rail
    void setRail(bool);
    // dice se treno sta andando sulle rail normali con limite di 80 o su quelle di transito
    bool onNormalRail() const;
    
    // Get the answer to request at station
    void sendStationRequest();
    // true se stazione gli dà il permesso di entrare in una rail, false se deve andare in parcheggio
    bool itCanTransit() const;
    //set if train is arrived to a station
    void setStop();
    // Set max speed
    void setMaxSpeed();
    // Set limited speed
    void setLimitedSpeed();
    // Has to be true if train is in station area (is setted by outside)
    bool isInStation() const;
    // Make the train enter station
    void enterStation();
    // Make the train exit station
    void exitStation();

    //tells if train has passed all the stations
    bool hasFinish() const;
    //set speed of train
    void setSpeed(double n);
    //increment Distance per minute
    void increaseDistance();
    //check if train is waiting in a station
    bool isWaiting();
    //check if train is just arrived
    bool justArrived() const;

    //return next station
    Station* NextStation() const;
    //return Distance from next Station
    int nextStationDistance() const;
    //train in range of 20 km
    //bool isArriving() const;

    //aggiora il delay in base a tempo di arrivo a stazione nextStation
    //confronta argomento con elemento in posizione timetableIndex
    void setDelay(int currentMinutes);
    // controlla se treno è almeno 20 minuti in anticipo considerando che
    // questo dato verrà controllato 20km prima di stazione
    bool inAnticipo(int currentMinutes) const;

    // dice se deve fermarsi alla stazione
    virtual bool hasToStop() const = 0;
public:
    virtual ~Train();
};

#endif // Train_h
