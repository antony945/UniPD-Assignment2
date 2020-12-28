#ifndef Train_h
#define Train_h
#include <iostream>
#include "Station.h"
#include <list>
#include <vector>

class Train {
protected:
    // Constructor
    Train(int, int, std::vector<int>);
    
    // Deve esssere settata nel momento di lettura da file line_description.txt
    static std::vector<Station> LINE_STATIONS;
    // Indice di stazione attuale
    int currentStationIndex;
    // TODO: Lista delle stazioni nelle quali deve fermarsi (forse inutili se tutti i treni si fermano in tutte le stazioni)
    // std::list<Station> currentTripStations;
    // Identificativo treno
    int id;
    // Verso di percorrenza di tratta (0 se normale, 1 se reverso)
    int startedFrom;
    // Velocità attuale treno
    int currentSpeed;
    // Ritardo attuale accumulato
    int currentDelay;
    // Distanza attuale percorsa da inizio del viaggio
    int currentDistance;
    // Orari di attuale corsa
    std::vector<int> timetable;

public:
    // Get list of all station
    static std::vector<Station> GET_LINE_STATIONS() { return LINE_STATIONS; }
    // Get ID
    int get_id() const { return id; }
    // Get startedFrom
    int get_startedFrom() const { return startedFrom; }
    // Get currentSpeed
    int get_currentSpeed() const { return currentSpeed; }
    // Get currentDelay
    int get_currentDelay() const { return currentDelay; }
    // Get currentDistance
    int get_currentDistance() const { return currentDistance; }
    // Get current timetable
    std::vector<int> get_timetable() const { return timetable; }

    virtual int get_maxSpeed() const = 0;
};

class RegionalTrain : public Train {
public:
    // Massima velocità del treno
    static const int MAX_SPEED = 160;
    // Ritorna massima velocità del treno
    int get_maxSpeed() const override { return MAX_SPEED; }

public:
    // Constructor
    RegionalTrain(int, int, std::vector<int>);
};

class HighSpeedTrain : public Train {
public:
    // Massima velocità del treno
    static const int MAX_SPEED = 240;
    // Ritorna massima velocità del treno
    int get_maxSpeed() const override { return MAX_SPEED; }

public:
    // Constructor
    HighSpeedTrain(int, int, std::vector<int>);
};

class SuperHighSpeedTrain : public Train {
public:
    // Massima velocità del treno
    static const int MAX_SPEED = 300;
    // Ritorna massima velocità del treno
    int get_maxSpeed() const override { return MAX_SPEED; }
    
public:
    // Constructor
    SuperHighSpeedTrain(int, int, std::vector<int>);
};

#endif // Train_h