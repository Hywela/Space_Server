#pragma once
class Space_Ship
{
public:
	Space_Ship(void);
	~Space_Ship(void);
private:
	int selected_hull;
	int selected_module;
	int energy;	
	int energyMax;
	bool isDestroyed;
};

