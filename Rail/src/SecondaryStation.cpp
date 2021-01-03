#include "SecondaryStation.h"

SecondaryStation::SecondaryStation():Station()
{
	//inizializzo i binari si transito
	Rail r1(true);
	Rail r2(false);
	transitRails.push_back(r1);
	transitRails.push_back(r2);
}

SecondaryStation::SecondaryStation(const std::string& name_, int distance_):Station(name_,distance_)
{
	Rail r1(true);			
	Rail r2(false);
	transitRails.push_back(r1);
	transitRails.push_back(r2);

}
bool SecondaryStation::isMain() {
	return false;
}

