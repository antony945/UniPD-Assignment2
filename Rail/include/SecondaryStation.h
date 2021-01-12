// Zoren Martinez 1230050 
#ifndef SecondaryStation_h
#define SecondaryStation_h
#include "Station.h"

class SecondaryStation : public Station {
    
public:
    SecondaryStation(const std::string& name_, int distance_);
    // Funzione per verificare se una funzione e' di tipo MainStation
    bool isMain() override;
    // Richiede un binario da occupare con un treno. Ritorna true se il binario e' stato occupato con successo, false i binari sono pieni
    bool railRequest(Train& myTrain) override;
    // Disabilito la semantica di copia e spostamento
    SecondaryStation(const SecondaryStation&) = delete;
    SecondaryStation(SecondaryStation&&) = delete;
    SecondaryStation& operator=(const SecondaryStation&) = delete;
    SecondaryStation& operator=(SecondaryStation&&) = delete;

private:
    // Di fatto, questi binari di transito non verranno mai occupati perche' nessun treno ci si fermera', ma al massimo ci transitera' senza occuparlo
    std::vector<Rail> transitRails;
    // In seguito infatti non li useremo ma sono stati dichiati 
};

#endif // SecondaryStation_h