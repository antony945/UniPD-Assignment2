#include "Rail.h"
#include <iostream>

Rail::Rail(bool dirOfTravel_) : occupied{ false }, dirOfTravel{ dirOfTravel_ }, trainId{ -999 }	{}
Rail::Rail() : occupied{ false }, dirOfTravel{ true }, trainId{ -999 }	{}	//direzione del binario settata in automatico a true(dall'origine al capolinea)


bool Rail::getTrainId() const{
	//invece di lanciare un eccezione, avviso e restituisco un valore inusuale: -999
	if (isOccupied() == false) std::cout << "Attenzione, il binario è libero. Nessun treno in sosta."; 
	
	return trainId;		
}

void Rail::setTrainId(int id){
	trainId = id;
}
void Rail::setOccupied(bool occupied_){
	if (!occupied_) trainId = -999;	//"cancello" l'id del treno che c'era prima (anche se in realta sostituisco con un valore inusuale) 

	occupied = occupied_;
}

bool Rail::getDirOfTravel() const{
	return dirOfTravel;
}

bool Rail::isOccupied() const{
	return occupied;
}
