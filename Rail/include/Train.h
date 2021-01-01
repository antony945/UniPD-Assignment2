#ifndef Train_h
#define Train_h
#include <vector>
#include <iostream>

class Station;

class Train {
protected:
    std::vector<Station*> stations; // Lista delle stazione PERCORSE (non necessariamente dove si ferma) dal treno
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
    Train(int, bool, const std::vector<Station*>&, const std::vector<int>&);
public:
    static const int MINIMUM_DISTANCE = 10;
    // Restituisce membri del treno
    bool isFromOrigin() const { return fromOrigin; }
    int getCurrentSpeed() const { return currentSpeed; }
    int getCurrentDelay() const { return currentDelay; }
    int getNextStationIndex() const { return nextStationIndex; }
    int getId() const { return id; }
    double getCurrentDistance() const { return currentDistance; }
    std::vector<Station*> getStations() const { return stations; }
    std::vector<int> getTimetable() const { return timetable; }
    virtual int getMaxSpeed() const = 0;

    virtual void trainInformation(std::ostream& os) const = 0;
    virtual std::string getType() const = 0;

    // Setta la distanza
    void addDistance(double toAdd) { currentDistance+=toAdd; }
    // Setta la velocità
    void setSpeed(int s) { currentSpeed = s; }
    // Controlla se c'è altra stazione
    bool hasNextStation() { return nextStationIndex!=(stations.size()-1); }
    // Setta indice stazione successiva
    void setNextStation() { nextStationIndex++; }
    // Dice se treno si deve fermare alla stazione oppure no
    virtual bool sendStopRequest(Station*) = 0;
};

#endif // Train_h