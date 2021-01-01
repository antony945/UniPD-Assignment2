#ifndef Station_h
#define Station_h
#include <vector>
#include <iostream>

class Station {
protected:
    // Nome stazione
    std::string name;
    // Distanza dall'origine
    int distance;
    // Binari standard
    // std::vector<Rail> standardRails;

public:
    Station(const std::string&, int);
    std::string getName() const { return name; }
    int getDistance() const { return distance; }
    void setDistance(int distance_) { distance = distance_; }

public:
    virtual bool isMain() const = 0;
    // virtual void incomingTrain() = 0;
    // virtual bool isFull() const = 0;
    // virtual void addTrain(const Train&, int) = 0;
};

#endif // Station_h
