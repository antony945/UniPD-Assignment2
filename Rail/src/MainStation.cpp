#include "MainStation.h"

MainStation::MainStation(const std::string& name_, int distance_):Station(name_,distance_)
{}

bool MainStation::isMain(){
	return true;
}