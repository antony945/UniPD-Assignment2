#include "SecondaryStation.h"
#include "Train.h"

// void SecondaryStation::useTransitRail(Train* myTrain) {

// 	//se il treno � puntiforme possiamo immaginare che il binario di transito venga usato per pochissimi istanti e poi liberata subito
// 	//per simulare quindi l'attraversamento occupo il binario e lo libero subito dopo

// 	if (myTrain->getLeft()) {	//controllo in che direzione sta andando
// 		transitRails[0].setOccupied(true);
// 		transitRails[0].setTrainId(myTrain->getId());
// 		transitRails[0].setOccupied(false);
// 	}
// 	else {
// 		transitRails[1].setOccupied(true);
// 		transitRails[1].setTrainId(myTrain->getId());
// 		transitRails[1].setOccupied(false);
// 	}
	
// }

SecondaryStation::SecondaryStation(const std::string& name_, int distance_):Station(name_,distance_)
{
	// Rail r1(true);			
	// Rail r2(false);
	// transitRails.push_back(r1);
	// transitRails.push_back(r2);
}


bool SecondaryStation::isMain() {
	return false;
}

