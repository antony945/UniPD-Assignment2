#include "Station.h"
#include <iostream>
#include "Rail.h"
#include "Train.h"
#include <vector>
#include <list>

//costruttore
Station::Station(const std::string& name_, int distanceLeft_) : name{ name_ }, distanceLeft{ distanceLeft_ } {
	// Inizializzo i 4 binari (negli indici pari vanno quelle dall'origine al capolinea, nei dispari i rimanenti
	
	// Inizializzo questa variabile anche se in seguito la modificheremo
	distanceRight = 0;

	for (int i = 0; i < 4; i++) {
		if (i % 2 == 0) {
			Rail tmp(true);
			standardRails.push_back(tmp);
		}
		else {
			Rail tmp(false);
			standardRails.push_back(tmp);
		}
	}
}

bool Station::parkEmpty() {
	return trainDeposit.size()==0;
}


bool Station::isFull(bool left) const{

	bool isFull = true;

	int i;
	if (left)
		i = 0;
	else
		i = 1;

	// For per controllare solo i binari di una certa direzione (Ricordiamo che i binari sono messi nell'array in maniera alternata in base alla loro direzione, vedi costruttore)
	for (; i < standardRails.size(); i = i + 2) {
		if (standardRails[i].isOccupied() == false)
			isFull = false;
	}

	return isFull;
}

void Station::depositTrain(Train& myTrain) {
	trainDeposit.push_back(&myTrain);
}

std::string Station::getName() const{
	return name;
}

int Station::getDistanceLeft() const{
	return distanceLeft;
}

int Station::getDistanceRight() const{
	return distanceRight;
}

void Station::manageParking(int currentMinutes) {
	// Controlla se treno in parcheggio deve partire
	int time_from_park_to_station = Train::STATION_AREA_KM/(Train::STATION_MAX_SPEED/60);

	// Finche' il deposito non e' vuoto e c'e' posto nei binari di sosta che vanno verso sinistra, controllo quale treno deve partire prima, in base alla sua timetable
	while(!isFull(true) && !trainDeposit.empty()) {
		Train* to_remove = nullptr;
		int min_starting_time = 1000000000;

		for(Train* t : trainDeposit) {
			if(t->getLeft() && t->hasToStart(currentMinutes+time_from_park_to_station)) {
				int i = t->nextStationTime();
				if(i < min_starting_time) {
					min_starting_time = i;
					to_remove = t;
				}
			}
		}

		// Se non c'e' nessun treno che voglia uscire
		if(to_remove==nullptr) {
			break;
		}

		// Devo far uscire il treno to_remove
		to_remove->sendStationRequest();
		to_remove->setParking(false);
		to_remove->setLimitedSpeed();
		trainDeposit.remove(to_remove);
	}

	// Finche' il deposito non e' vuoto e c'e' posto nei binari di sosta che vanno verso destra, controllo quale treno deve partire prima, in base alla sua timetable
	while(!isFull(false) && !trainDeposit.empty()) {

		Train* to_remove = nullptr;
		int min_starting_time = 1000000000;

		for(Train* t : trainDeposit) {
			if(!t->getLeft() && t->hasToStart(currentMinutes+time_from_park_to_station)) {
				int i = t->nextStationTime();
				if(i < min_starting_time) {
					min_starting_time = i;
					to_remove = t;
				}
			}
		}

		// Se non c'è nessun treno che voglia uscire
		if(to_remove==nullptr) {
			break;
		}

		// Devo far uscire il treno to_remove
		to_remove->sendStationRequest();
		to_remove->setParking(false);
		to_remove->setLimitedSpeed();
		trainDeposit.remove(to_remove);
	}

}

void Station::freeRail(Train& myTrain) {
	// Controllo se l'ID del treno corrisponde a quello presente nel binario
	if(myTrain.getId() == standardRails[myTrain.getStationRail()].getTrainId()) {
		std::cout << this->getName() << ": binario liberato, id = " << standardRails[myTrain.getStationRail()].getTrainId() << "\n";
		// Libero il binario
		standardRails[myTrain.getStationRail()].setOccupied(false);
		standardRails[myTrain.getStationRail()].setTrainId(-999);
		// Tolgo il riferimento del binario al treno
		myTrain.setStationRail(-1);
	}
}

Station::~Station() = default;