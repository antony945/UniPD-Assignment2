#ifndef Station_h
#define Station_h

class Station {
protected:
    // Station* prevStation;   // Settata da file line_description.txt
    // Station* nextStation;   // Settata da file line_description.txt
    // int distanceFromOrigin; // Settata da file line_description.txt
};

class LocalStation : public Station {

};

class MainStation : public Station {

};

#endif // Station_h