// Zoren Martinez 1230050
#ifndef MainStation_h
#define MainStation_h
#include "Station.h"

class MainStation : public Station {

public:
	MainStation(const std::string& name_, int distance_);
	// Funione per verificare se la stazione e' di tipo MainStation
	bool isMain() override;
	// Richiede un binario da occupare con un treno. Ritorna true se il binario e' stato occupato con successo, false i binari sono pieni
	bool railRequest(Train& myTrain) override;
	// Disabilito la semantica di copia e spostamento
	MainStation(const MainStation&) = delete;
	MainStation(MainStation&&) = delete;
	MainStation& operator=(const MainStation&) = delete;
	MainStation& operator=(MainStation&&) = delete;

};

#endif // MainStation_h