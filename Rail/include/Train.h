#ifndef Train_h
#define Train_h
#include <iostream>
#include "Station.h"
#include <list>
#include <vector>

class Train {
public:
    // static std::vector<Station> STATION_LIST;   // Deve esssere settata nel momento di lettura da file line_description.txt
    // Station currentStation;
    // std::list<Station> currentTripStations; // Lista delle stazioni nelle quali deve fermarsi

    int ID; // Identificativo treno
    int speed;  // Velocità attuale treno
    std::vector<int> times; // Orari di attuale corsa
protected:
    Train(int ID_, int speed_, std::vector<int> times_) : ID{ID_}, speed{speed_}, times{times_} {}

    // void aggiungiCorsa();
};

class RegionalTrain : public Train {
public:
    RegionalTrain(int ID_, int speed_, std::vector<int> times_) : Train(ID_, speed_, times_) {}
    static const int MAX_SPEED = 160;   // Massima velocità per questo tipo di treno
};

class HighSpeedTrain : public Train {
public:
    HighSpeedTrain(int ID_, int speed_, std::vector<int> times_) : Train(ID_, speed_, times_) {}
    static const int MAX_SPEED = 240;   // Massima velocità per questo tipo di treno
};

class SuperHighSpeedTrain : public Train {
public:
    SuperHighSpeedTrain(int ID_, int speed_, std::vector<int> times_) : Train(ID_, speed_, times_) {}
    static const int MAX_SPEED = 300;   // Massima velocità per questo tipo di treno
};

#endif // Train_h