#ifndef Train_h
#define Train_h
#include <vector>
#include "Station.h"

class Train {
protected:
    static std::vector<Station> stations;
    static const int MINIMUM_DISTANCE = 10;

    std::vector<int> timetable;
    const int MAX_SPEED;
    int currentDelay;
    int id;
    bool left;
    int currentSpeed;
    int currentDistance;
    // int currentTime;
    // Station nextStation;
    int currentStationIndex;

public:
    // Costruttore
    Train(int id_, bool left_, const std::vector<int>& timetable_);
    
    // Restituisce membri del treno
    int getCurrentSpeed() const;
    int getCurrentDelay() const;
    int getId() const;
    int getCurrentDistance() const;
    
    // Metodo statico che prende vettore di stazioni in input
    static void setStations(const std::vector<Station>& stations_);

    // Dice se treno si deve fermare alla stazione oppure no
    // virtual void sendRequest() = 0;
};

#endif // Train_h
