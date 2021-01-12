#include "MainStation.h"
#include "Train.h"

MainStation::MainStation(const std::string& name_, int distance_):Station(name_,distance_)
{}


bool MainStation::railRequest(Train& myTrain) {

	// Direzione del treno
	bool trainDir = myTrain.getLeft();

	// Indico al treno che andra' in un binario in cui la massima velocita' e' 80 km/h
	myTrain.setRail(true);

	// Se per la direzione specificata i binari sono pieni, ritorno false
	if (isFull(trainDir)) return false;

	int i;

	if (trainDir)
		i = 0;
	else
		i = 1;

	// In base alla direzione specificata occupo il primo binario libero con il treno passato per parametro
	for (; i < standardRails.size(); i = i + 2) {
		if (standardRails[i].isOccupied() == false) {
			standardRails[i].setTrainId(myTrain.getId());
			standardRails[i].setOccupied(true);
			myTrain.setStationRail(i);
			// Esco dal for per occupare un solo binario
			break;
		}
	}

	// Restituisco true per indicare che l'occupazione di un binario e' andato con successo
	return true;
}

bool MainStation::isMain(){
	return true;
}