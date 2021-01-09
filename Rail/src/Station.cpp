#include "Station.h"
#include <iostream>
#include "Rail.h"
#include "Train.h"
#include <vector>
#include <list>


//costruttore di default di default
Station::Station() : name{ "" }, distance{ 0 } {

	//inizializzo i 4 binari (nelle posizioni pari vanno quelle dall'orgine al capolinea, nei dispari i rimanenti)
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

Station::Station(const std::string& name_, int distance_) : name{ name_ }, distance{ distance_ } {
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

bool Station::railRequest(Train* myTrain) {
	if(myTrain->hasToStop()) {
		// Se si deve fermare dì al treno di passare su binari normali
		myTrain->setRail(true);

		bool trainDir = myTrain->getLeft();	//direzione del treno

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
	for(Train* t : trainDeposit) {
		if(t->getId() == myTrain->getId()) return;
	}

	trainDeposit.push_back(myTrain);
}

std::string Station::getName() const{
	return name;
}
int Station::getDistance() const{
	return distance;
}

void Station::manageParking(int currentMinutes) {
	// Controlla se treno in parcheggio deve partire
	int time_from_park_to_station = 5/(80/60);

	// Prima tira fuori da stazione i treni super alta velocità che devono partire
	for(Train* t : trainDeposit) {
		if(t->isSuperAV() && t->hasToStart(currentMinutes+time_from_park_to_station)) {
			t->sendStationRequest();
			if(t->itCanTransit())
				trainDeposit.remove(t);
		} 
	}

	// Poi tira fuori da stazione i treni alta velocità che devono partire
	for(Train* t : trainDeposit) {
		if(t->isAV() && t->hasToStart(currentMinutes+time_from_park_to_station)) {
			t->sendStationRequest();
			if(t->itCanTransit())
				trainDeposit.remove(t);
		}
	}

	// Infine tira fuori da stazione i treni regionali che devono partire
	for(Train* t : trainDeposit) {
		if(t->isRegional() && t->hasToStart(currentMinutes+time_from_park_to_station)) {
			t->sendStationRequest();
			if(t->itCanTransit())
				trainDeposit.remove(t);
		}
	}
}

void Station::freeRail(Train *t) {
    bool done=false;
    int i=0;
    while(!done && i<standardRails.size()){
        if(standardRails[i].getTrainId()==(t->getId())) {
            standardRails[i].setOccupied(false);
            standardRails[i].setTrainId(-999);
        }
    }
}
