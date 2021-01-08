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
	return true;						//il treno ha ricevuto un binario su cui fermarsi
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
	trainDeposit.push_back(myTrain);
}

std::string Station::getName() const{
	return name;
}
int Station::getDistance() const{
	return distance;
}
