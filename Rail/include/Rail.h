#ifndef Rail_h
#define Rail_h

class Rail {
private:
	bool occupied;		//true se il binario � occupato, false altrimenti
	bool dirOfTravel;	//direzione in cui va percorso il binario, true se dall'origine al capolinea, false altrimenti
	int trainId;		//id del treno che lo sta occupando, -999 il binario � libero

public:
	bool isOccupied() const;
	bool getDirOfTravel() const;
	int getTrainId() const;
	void setTrainId(int id);
	void setOccupied(bool occupied_);
	Rail(bool dirOfTravel_);
	Rail();
};

#endif // Rail_h