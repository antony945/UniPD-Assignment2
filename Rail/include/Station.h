#ifndef Station_h
#define Station_h
#include <iostream>
#include "Rail.h"
#include "Train.h"
#include <vector>

class Station {
protected:
    // Nome stazione
    std::string name;
    // Distanza dall'origine
    int distance;
    // Binari standard
    std::vector<Rail> standardRails;
    //Deposito treni
    std::vector<Train> trainDeposit;
public:
    Station();
    Station(const std::string& name_, int distance_);
    bool isFull() const;
    std::string getName() const;
    int getDistance() const;
    bool railRequest(const Train& myTrain);        //metodo per chiedere se ci sono binari disponibili per il treno (ancora da fare)
    void depositTrain(const Train& myTrain);       //quando la richiesta per un binario di un treno non può essere soddisfatta, il treno viene messo nel deposito
    virtual bool isMain() = 0;

    Station(const Station&) = delete;
    Station(Station&&) = delete;
    Station& operator=(const Station&) = delete;
    Station& operator=(Station&&) = delete;
    
};

#endif // Station_h
