#ifndef MainStation_h
#define MainStation_h
#include "Station.h"

class MainStation : public Station {

public:
	MainStation(const std::string& name_, int distance_);
	MainStation();

private:
	bool isMain() override;
};

#endif // MainStation_h