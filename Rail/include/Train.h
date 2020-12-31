#ifndef Train_h
#define Train_h
#include <vector>
#include <iostream>

class Station;

class Train {
protected:
    static std::vector<Station*> ALL_STATIONS;
protected:
    std::vector<int> timetable; // Devono avere stesso numeri di elementi di myStation
    int currentDelay;
    int id;
    bool fromOrigin;
    int currentSpeed;
    double currentDistance;
    // Contiene indice prossima stazione da visitare (si aggiorna ogni volta che parte da una stazione)
    int nextStationIndex;

protected:
    // Costruttore
    Train(int, bool, const std::vector<int>&);
public:
    static const int MINIMUM_DISTANCE = 10;
    // Restituisce membri del treno
    bool isFromOrigin() const { return fromOrigin; }
    int getCurrentSpeed() const { return currentSpeed; }
    int getCurrentDelay() const { return currentDelay; }
    int getNextStationIndex() const { return nextStationIndex; }
    int getId() const { return id; }
    double getCurrentDistance() const { return currentDistance; }
    std::vector<int> getTimetable() const { return timetable; }
    virtual int getMaxSpeed() const = 0;

    // Setta la distanza
    void addDistance(double toAdd) { currentDistance+=toAdd; }
    // Setta la velocità
    void setSpeed(int s) { currentSpeed = s; }
    // Setta indice stazione
    void setNextStation(int i) { nextStationIndex = i; }
    // Controlla se mancano 20 km a prossima stazione
    bool nearNextStation() {
        if(currentDistance >= ALL_STATIONS[nextStationIndex]->getDistance()-20)
            return true;
        return false;
    }
    // Dice se treno si deve fermare alla stazione oppure no
    // virtual void sendRequest() = 0;
};

#endif // Train_h