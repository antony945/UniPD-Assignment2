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
    // Dice se treno può transitare in stazione oppure è in parcheggio
    bool canTransit;
    // true = normalRail con limite 80, false = transitRail senza limite
    bool normalRail;
    // dice se treno ha finito completamente corsa
    bool end;
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
    // Setta la rail sulla quale il treno andrà dentro la stazione (true se rail con limite di 80, false se quella di transito)
    void setRail(bool);
    // dice se treno sta andando sulle rail normali con limite di 80 o su quelle di transito
    bool onNormalRail() const;
    
    // Manda e riceve la risposta alla domanda di fermata o transito da parte del treno alla stazione 
    void sendStationRequest();
    // Permetti al treno di passare o meno
    void setTransit(bool b);
    // ritorna true se treno ha il permesso da stazione di entrare in una rail, false se deve andare in parcheggio
    bool itCanTransit() const;
    // setta velocità a 0
    void setStop();
    // Setta velocità massimo per quel particolare tipo di treno
    void setMaxSpeed();
    // Setta velocità limitata da stazione (80 km/h)
    void setLimitedSpeed();
    // Ritorna vero se il treno è nell'area della stazione (tra 5km prima e 5km dopo) (viene settata dalla railway)
    bool isInStation() const;
    // Fa si che il treno entri nella stazione
    void enterStation();
    // Fa si che il treno esca dalla stazione e si aumenti nextStationIndex
    void exitStation();

    // Dice se il treno ha passato tutte le stazioni
    bool hasFinish() const;
    
    bool getEnd() const {
        return end;
    }

    void setEnd() {
        end = true;
    }
    // Setta velocità del treno
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

    //aggiorna il delay in base a tempo di arrivo a stazione nextStation
    //confronta argomento con elemento in posizione timetableIndex
    void setDelay(int currentMinutes);
    // controlla se treno è almeno 20 minuti in anticipo considerando che
    // questo dato verrà controllato 20km prima di stazione
    bool inAnticipo(int currentMinutes) const;

    // dice se deve già prendere i passeggeri oppure no
    bool hasToStart(int currentMinutes) const;

    // dice se deve fermarsi alla stazione nextStation
    virtual bool hasToStop() const = 0;
public:
    virtual ~Train();
};

#endif // Train_h
