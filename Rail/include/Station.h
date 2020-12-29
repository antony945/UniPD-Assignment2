#ifndef Station_h
#define Station_h
#include <iostream>
#include "Rail.h"
#include <vector>

class Station {
protected:
    // Nome stazione
    std::string name;
    // Distanza dall'origine
    int distance;
    // Binari standard
    std::vector<Rail> standardRails;
public:
    Station(const std::string& name_, int distance_);
    
    std::string getName();
    int getDistance();

public:
    virtual bool isMain() = 0;
    virtual bool isFull() = 0;
};

#endif // Station_h
