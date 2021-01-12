#include "Rail.h"
#include <iostream>

Rail::Rail(bool dirOfTravel_) : occupied{ false }, dirOfTravel{ dirOfTravel_ }, trainId{ -999 }	{}
Rail::Rail() : occupied{ false }, dirOfTravel{ true }, trainId{ -999 }	{}	//direzione del binario settata in automatico a true(dall'origine al capolinea)

int Rail::getTrainId() const{
	// Invece di lanciare un eccezione, avviso e restituisco un valore inusuale: -999
	return trainId;
}

void Rail::setTrainId(int id){
	trainId = id;
}

void Rail::setOccupied(bool occupied_){
	
	// Quando si vuole rimuovere l'ID del treno che occupava questo binario, sostituisco il suo ID con un valore 'inusuale': -999
	if (!occupied_) trainId = -999;
	occupied = occupied_;
}

bool Rail::getDirOfTravel() const{
	return dirOfTravel;
}

bool Rail::isOccupied() const{
	return occupied;
}
