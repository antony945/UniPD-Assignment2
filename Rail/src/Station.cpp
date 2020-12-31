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

bool Station::isFull() {

	bool isFull = true;

	for (int i = 0; i < standardRails.size(); i++) {	//controllo se è presente un binario libero
		if (standardRails[i].isOccupied() == false)
			isFull = false;
	}

	return isFull;
}

std::string Station::getName() {
	return name;
}
int Station::getDistance() {
	return distance;
}
