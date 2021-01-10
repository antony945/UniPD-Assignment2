#include "Railway.h"
#include <vector>

int absoluteValue(int x, int y);

void Railway::checkMinimumDistance() {

	int MIN_DIS = 15;		//distanza minima da rispettare (ho messo 15km per avere un margine: il treno più veloce da 300km/h può fare al massimo 5km al minuto.)

	for (int i = 0; i < trains.size(); i++) {
		for (int j = i + 1; j < trains.size(); j++) {
			//controllo prima se i due treni stanno andando sullo stesso binario, poi se sono troppo vicini
			if ((trains[i]->getLeft() == trains[j]->getLeft()) && (absoluteValue(trains[i]->getCurrentDistance(), trains[j]->getCurrentDistance()) < MIN_DIS)) {

				//se il treno in trains[i] e' piu' veloce del treno in [j] e si trova anche dietro ad esso, riduco la velocità di trains[i]
				if (trains[i]->getCurrentSpeed() > trains[j]->getCurrentSpeed() && trains[i]->getCurrentDistance() < trains[j]->getCurrentDistance()) {
					trains[i]->setSpeed(trains[j]->getCurrentSpeed());
				}
				//se il treno in trains[j] e' piu' veloce del treno in [i] e si trova anche dietro ad esso, riduco la velocità di trains[j]
				else if (trains[i]->getCurrentSpeed() < trains[j]->getCurrentSpeed() && trains[j]->getCurrentDistance() < trains[i]->getCurrentDistance()) {
					trains[j]->setSpeed(trains[i]->getCurrentSpeed());
				}
				//altrimeni anche se sono vicini quello che è più veloce sta davanti e non ci sono problemi
			}
		}

	}

}

int absoluteValue(int x, int y) {	//funzione per calcolare il valore assoluto della sottrazione

	int diff = x > y ? x - y : y - x;

	return diff;
}