#ifndef Station_h
#define Station_h
#include <iostream>
#include "Rail.h"
#include <vector>
#include <list>

class Train;

class Station {
protected:
    // Nome stazione
    std::string name;
    // Distanza dall'origine
    int distance;
    // Binari standard
    std::vector<Rail> standardRails;
    //Deposito treni
    std::list<Train*> trainDeposit;
public:
    Station();
    Station(const std::string& name_, int distance_);


    bool isFull(bool left) const;                   //metodo per vedere se i binari in una certa direzione sono pieni, left serve per capire quale direzione si vuole controllare, true verso sinistra, false verso destra
    std::string getName() const;
    int getDistance() const;
    bool railRequest(const Train& myTrain);        //metodo per chiedere se ci sono binari disponibili per il treno //restituisce false se i binari sono pieni
    void depositTrain(Train* myTrain);       //quando la richiesta per un binario di un treno non pu� essere soddisfatta, il treno viene messo nel deposito
    virtual bool isMain() = 0;

    Station(const Station&) = delete;
    Station(Station&&) = delete;
    Station& operator=(const Station&) = delete;
    Station& operator=(Station&&) = delete;
    
};

#endif // Station_h
