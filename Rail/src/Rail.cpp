#include "Rail.h"
#include <iostream>

Rail::Rail(bool dirOfTravel_) : occupied{ false }, dirOfTravel{ dirOfTravel_ }, trainId{ -999 }	{}

bool Rail::getTrainId() {
	//invece di lanciare un eccezione, avviso e restituisco un valore inusuale: -999
	if (isOccupied() == false) std::cout << "Attenzione, il binario è libero. Nessun treno in sosta."; 
	
	return trainId;		
}

bool Rail::getDirOfTravel() {
	return dirOfTravel;
}

bool Rail::isOccupied() {
	return occupied;
}
