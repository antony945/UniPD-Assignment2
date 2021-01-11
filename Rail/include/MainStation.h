#ifndef MainStation_h
#define MainStation_h
#include "Station.h"

class MainStation : public Station {

public:
	MainStation(const std::string& name_, int distance_);
	bool isMain() override;

	MainStation(const MainStation&) = delete;
	MainStation(MainStation&&) = delete;
	MainStation& operator=(const MainStation&) = delete;
	MainStation& operator=(MainStation&&) = delete;

};

#endif // MainStation_h