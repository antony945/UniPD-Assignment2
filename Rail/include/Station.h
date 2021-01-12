// Zoren Martinez 1230050
#ifndef Station_h
#define Station_h
#include <iostream>
#include "Rail.h"
#include <vector>
#include <list>

class Train;

class Station {
protected:
    // Nome stazione
    std::string name;
    // Utilizziamo due variabili per distinguere le distanze dalla stazione d'origine o di capolinea
    // La prima rispetto all'origine
    int distanceLeft;
    // La seconda rispetto al capolinea
    int distanceRight;
    // Binari standard
    std::vector<Rail> standardRails;
    //Deposito treni
    std::list<Train*> trainDeposit;
public:
    Station(const std::string& name_, int distanceLeft_);
    // Setta la distanza di questa stazione rispetto al capolinea
    void setDistanceRight(int distanceRight_) { distanceRight = distanceRight_; }
    // Libera il binario se il treno t lo sta occupando
    void freeRail(Train& t);
    // Gestisce i treni parcheggiati nel deposito
    void manageParking(int currentMinutes);
    // In base alla direzione scelta, controlla se ci sono binari di sosta disponibili. true verso sinistra, false verso destra
    bool isFull(bool left) const;       
    // Restituisce il nome della stazione
    std::string getName() const;
    // Restituisce la distanza dalla stazione di origine
    int getDistanceLeft() const;
    // Restituisce la distanza dalla stazione capolinea
    int getDistanceRight() const;
    // Quando la richiesta di un binario per un treno non puo' essere soddisfatta, il treno viene messo nel deposito
    void depositTrain(Train& myTrain);       
    // Funzione virtuale per capire se una stazione e' di tipo Principale(MainStation) o Secondaria(SecondaryStation)
    virtual bool isMain() = 0;
    // Controlla se il deposito dei treni e' vuoto
    bool parkEmpty();
    // Richiede un binario da occupare con un treno. Ritorna true se il binario e' stato occupato con successo, false i binari sono pieni
    virtual bool railRequest(Train& myTrain) = 0;
    // Disabilito la semantica di copia e spostamento
    Station(const Station&) = delete;
    Station(Station&&) = delete;
    Station& operator=(const Station&) = delete;
    Station& operator=(Station&&) = delete;
};

#endif // Station_h
