#pragma once
class Modul
{
	private:
	int nameId;			//Refrence to string arrayslot
	int maxHealth;		//Maximum health level
	int currentHealth;	//Current health level
	int currentPower;	//Current power stored
	int disabled;		//Turns disabled
	int accuracy;		//Chance to hit
	bool hovered;
	bool held;

	int requiredPower;	//Power required to use
	int damage;			//Damage this gun does
	int disablePower;	//Turns this gun may disable on hit

public:
	Modul(void);
	~Modul(void);
};

