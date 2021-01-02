#ifndef Rail_h
#define Rail_h

class Rail {
protected:
	bool occupied;		//true se il binario è occupato, false altrimenti
	bool dirOfTravel;	//direzione in cui va percorso il binario, true se dall'origine al capolinea, false altrimenti
	int trainId;		//id del treno che lo sta occupando, -999 il binario è libero

public:
	bool isOccupied() const;
	bool getDirOfTravel() const;
	bool getTrainId() const;
	Rail(bool dirOfTravel_);
	Rail();

	Rail(const Rail&) = delete;
	Rail(Rail&&) = delete;
	Rail& operator=(const Rail&) = delete;
	Rail& operator=(Rail&&) = delete;
};

#endif // Rail_h