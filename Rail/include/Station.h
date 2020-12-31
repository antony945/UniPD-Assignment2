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
    bool isFull();
    std::string getName();
    int getDistance();
    bool railRequest(Train myTrain);    //metodo per chiedere se ci sono binari disponibili per il treno    (ancora non implementato)
    virtual bool isMain() = 0;
    
};

#endif // Station_h
