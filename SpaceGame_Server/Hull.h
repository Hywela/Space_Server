#pragma once
class Hull
{
	int armor;			//The armor value of the chassis piece.
	int specialAttack;	//Stat to boost attack units in this tile.
	int specialDefence;	//Stat to boost defencive units in this tile.
	bool isLocked;		//If the tile is attached to something.
	bool isEmpty;		//If there is a tile here
	bool isPlaceable;	//If you can place a tile here
	int numAnchors;		//Number of sides connected

public:
	Hull(void);
	~Hull(void);
};

