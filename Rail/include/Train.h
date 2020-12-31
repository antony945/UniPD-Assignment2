#ifndef Train_h
#define Train_h
#include <vector>
#include <iostream>

class Station;

class Train {
protected:
    static std::vector<Station*> ALL_STATIONS;
protected:
    static const int MINIMUM_DISTANCE = 10;

    std::vector<int> timetable; // Devono avere stesso numeri di elementi di myStation
    int currentDelay;
    int id;
    bool fromOrigin;
    int currentSpeed;
    double currentDistance;
    int currentStationIndex;

protected:
    // Costruttore
    Train(int, bool, const std::vector<int>&);
public:
    // Restituisce membri del treno
    int getCurrentSpeed() const { return currentSpeed; }
    int getCurrentDelay() const { return currentDelay; }
    int getId() const { return id; }
    double getCurrentDistance() const { return currentDistance; }
    std::vector<int> getTimetable() const { return timetable; }
    virtual int getMaxSpeed() const = 0;

    // Setta la distanza
    void addDistance(double toAdd) { currentDistance+=toAdd; }
    // Setta la velocità
    void setSpeed(int s) { currentSpeed = s; }
    // Setta indice stazione
    void setIndexStation(int i) { currentStationIndex = i; }
    // Controlla se mancano 20 km a prossima stazione
    bool nearNextStation() {
        if(currentDistance >= ALL_STATIONS[currentStationIndex+1]->getDistance()-20)
            return true;
        return false;
    }
    // Dice se treno si deve fermare alla stazione oppure no
    // virtual void sendRequest() = 0;
};

#endif // Train_h