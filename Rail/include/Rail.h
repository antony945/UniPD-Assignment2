//  Zoren Martinez 1230050
#ifndef Rail_h
#define Rail_h

class Rail {
private:
	// A true se il binario e' occupato, false altrimenti
	bool occupied;		
	// Direzione in cui va percorso il binario, true se dall'origine al capolinea, false altrimenti
	bool dirOfTravel;	
	// ID del treno che lo sta occupando, -999 il binario e' libero
	int trainId;		

public:
	// Restituisce true se il binario e' occupato, false altrimenti
	bool isOccupied() const;
	// Restuisce la direzione del binario
	bool getDirOfTravel() const;
	// Restituisce l'ID del treno che sta occupando questo binario
	int getTrainId() const;
	// Inserisce l'ID del treno che occupera' questo binario
	void setTrainId(int id);
	// Set per la variabile che tiene conto se questo binario e' occupato
	void setOccupied(bool occupied_);

	Rail(bool dirOfTravel_);
	Rail();
};

#endif // Rail_h