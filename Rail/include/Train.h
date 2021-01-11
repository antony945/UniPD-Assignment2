//Alberto Castagnaro 1219641
#ifndef Train_h
#define Train_h
#include <vector>
#include <string>

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
    double currentDistance;
    //Index that represent the next Station where the train is going
    int nextStationIndex;
    //Timetable index
    int timetableIndex;
    //True if train is in station area (tra 5km prima e 5km dopo)
    bool inStation;
    //time stopped in Station
    int stationStopTime;
    // Dice se treno può transitare in stazione oppure è in parcheggio
    bool canTransit;
    // true = normalRail con limite 80, false = transitRail senza limite
    bool normalRail;
    // dice se treno ha finito completamente corsa
    bool end;
    // dice se treno è in parcheggio al momento
    bool parked;
    bool firstTimePre20;
    bool firstTime0;
    bool firstTimeArrived;
    int stationRail;
public:
    // Constructors
    Train(int id_, bool left_,const std::vector<Station*>& stations_, int maxSpeed, const std::vector<int>& timetable_);
    Train(const Train&) = delete;
    Train& operator=(const Train&) = delete;
    Train(Train&&) = delete;
    Train& operator=(Train&&) = delete;
    
    void park();
    // getter - functions

    void setStationRail(int r) {
        stationRail = r;
    }
    int getStationRail() const {
        return stationRail;
    }
    bool isParked() const;
    std::vector<int> getTimetable() const;
    std::vector<Station*> getStations() const { return stations; }
    int getNextStationIndex() const;
    int getTimetableIndex() const;
    double getCurrentSpeed() const;
    bool getLeft() const;
    int getCurrentDelay() const;
    int getId() const;
    double getCurrentDistance() const; //KM
    // Setta la rail sulla quale il treno andrà dentro la stazione (true se rail con limite di 80, false se quella di transito)
    void setRail(bool);
    // dice se treno sta andando sulle rail normali con limite di 80 o su quelle di transito
    bool onNormalRail() const;
    
    int nextStationTime() const {
        return timetable[timetableIndex];
    }

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

    bool firstTimePre20km() const {
        return firstTimePre20;
    }
    bool firstTime0km() const {
        return firstTime0;
    }
    void setFirstTimePre20km(bool b) {
        firstTimePre20 = b;
    }
    void setFirstTime0km(bool b) {
        firstTime0 = b;
    }
    void setFirstTimeArrived(bool b) {
        firstTimeArrived = b;
    }

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
    // Incrementa distanza di un minuto a seconda della velocità del treno
    void increaseDistance();
    // Controlla se il treno sta aspettando i passeggeri alla stazione oppure no
    bool isWaiting();
    // Controlla se il treno è appena arrivato alla stazione
    bool justArrived() const;
    // Parcheggia il treno
    void setParking(bool);

    //return next station
    Station& nextStation() const;
    //return Distance from next Station in km
    double nextStationDistance() const;

    //aggiorna il delay in base a tempo di arrivo a stazione nextStation
    //confronta argomento con elemento in posizione timetableIndex
    void setDelay(int currentMinutes);
    // controlla se treno è almeno 20 minuti in anticipo considerando che
    // questo dato verrà controllato 20km prima di stazione
    bool inAnticipo(int currentMinutes) const;

    // dice se deve già prendere i passeggeri oppure no
    bool hasToStart(int currentMinutes) const;

    virtual void checkTimetable() = 0;
    // dice se deve fermarsi alla stazione nextStation
    virtual bool hasToStop() const = 0;
    // dice di che tipo è il treno
    virtual std::string getType() const = 0;
public:
    virtual ~Train();
};

#endif // Train_h
