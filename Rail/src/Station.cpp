#include "Station.h"
#include <iostream>
#include "Rail.h"
#include <vector>

//costruttore di default di default
Station::Station() : name{ "" }, distance{ 0 } {

	//inizializzo i 4 binari (nelle posizioni pari vanno quelle dall'orgine al capolinea, nei dispari i rimanenti
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

bool Station::isFull() const{	//non ancora definitivo... in questo modo vengono controllati anche i binari della direzione opposta. sarà da aggiungere ai treni anche una variabile per capire in che direzione devono andare

	bool isFull = true;

	for (int i = 0; i < standardRails.size(); i++) {	//controllo se è presente un binario libero
		if (standardRails[i].isOccupied() == false)
			isFull = false;
	}

	return isFull;
}

void Station::depositTrain(const Train& myTrain) {
	trainDeposit.push_back(myTrain);
}

std::string Station::getName() const{
	return name;
}
int Station::getDistance() const{
	return distance;
}
