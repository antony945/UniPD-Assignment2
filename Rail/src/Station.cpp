#include "Station.h"
#include <iostream>
#include "Rail.h"
#include "Train.h"
#include <vector>
#include <list>

//costruttore
Station::Station(const std::string& name_, int distanceLeft_) : name{ name_ }, distanceLeft{ distanceLeft_ } {
	//inizializzo i 4 binari (negli indici pari vanno quelle dall'orgine al capolinea, nei dispari i rimanenti
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

bool Station::railRequest(Train* myTrain) {
	if(myTrain->hasToStop()) {
		// Se si deve fermare dì al treno di passare su binari normali
		myTrain->setRail(true);

		bool trainDir = myTrain->getLeft();	//direzione del treno

		// if(!parkEmpty()) return false;
		if (isFull(trainDir)) return false;	//se � pieno ritorno false, sar�  compito  di railway chiamare la funzione depositTrain per mettere il treno nel deposito
		
		int i;
		
		if (trainDir)
			i = 0;
		else
			i = 1;

		for (; i < standardRails.size(); i = i+2) {
			if (standardRails[i].isOccupied() == false) {
				standardRails[i].setTrainId(myTrain->getId());
				standardRails[i].setOccupied(true);
				myTrain->setStationRail(i);
				std::cout << this->getName() << ": binario ora occupato, id = " << standardRails[i].getTrainId() << "\n";
				break;						//esco dal for
			}
		}
	} else {
		// Se non si deve fermare dì al treno di passare su binari di transito
		myTrain->setRail(false);
		// Non servono altri controlli, potrà sempre passare e non serve "riempire" il binario di transito
	}

	return true;						//il treno ha ricevuto un binario su cui fermarsi/passare
}

bool Station::isFull(bool left) const{	//controlla se i binari in una certa direzione sono pieni, left a true guarda gli indici pari, false quelli dispari

	bool isFull = true;

	int i;
	if (left)
		i = 0;
	else
		i = 1;

	for (; i < standardRails.size(); i = i + 2) {	//controllo se � presente un binario libero
		if (standardRails[i].isOccupied() == false)
			isFull = false;
	}

	return isFull;
}

void Station::depositTrain(Train* myTrain) {
	// Controlla se treno è gia presente in deposito, se si non aggiungerlo
	// Controllo viene fatto in altra funzione
	trainDeposit.push_back(myTrain);
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
	int time_from_park_to_station = 5/(80/60);

	// Finchè la stazione non è piena e il parcheggio non è vuoto
	// Scorro tutti i treni, vedo quello che avrebbe la timetable corrente prima di tutti gli altri
	// A quello li lo faccio entrare dentro
	// std::cout << getName() << '\n';
	// if(!isFull(true) && !trainDeposit.empty()) {
	// 	std::cout << "stazione non piena e deposito non vuoto\n";
	// } else {
	// 	std::cout << "stazione piena o deposito vuoto\n";
	// }

	// controllo i treni nel deposito a sinistra
	while(!isFull(true) && !trainDeposit.empty()) {
		// std::cout << "controllo se ci sono treni a sx da fare uscire\n";

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

		// Se non c'è nessun treno che voglia uscire
		if(to_remove==nullptr) {
			// std::cout << "treni parcheggiati a sx non vogliono uscire\n";
			break;
		}

		// Devo far uscire il treno to_remove
		to_remove->sendStationRequest();
		to_remove->setParking(false);
		to_remove->setLimitedSpeed();
		trainDeposit.remove(to_remove);
	}

	// controllo i treni nel deposito a destra
	while(!isFull(false) && !trainDeposit.empty()) {
		// std::cout << "controllo se ci sono treni a dx da fare uscire\n";

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
			// std::cout << "treni parcheggiati a dx non vogliono uscire\n";
			break;
		}

		// Devo far uscire il treno to_remove
		to_remove->sendStationRequest();
		to_remove->setParking(false);
		to_remove->setLimitedSpeed();
		trainDeposit.remove(to_remove);
	}

}

void Station::freeRail(Train *t) {
	std::cout << this->getName() << ": binario liberato, id = " << standardRails[t->getStationRail()].getTrainId() << "\n";
	standardRails[t->getStationRail()].setOccupied(false);
	standardRails[t->getStationRail()].setTrainId(-999);
	t->setStationRail(-1);
}
