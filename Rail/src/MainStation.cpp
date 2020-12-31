#include "MainStation.h"


bool MainStation::isMain(){
	return true;
}

MainStation::MainStation(const std::string& name_, int distance_):Station(name_,distance_)
{}
MainStation::MainStation() : Station()
{}