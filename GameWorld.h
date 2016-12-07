#pragma once

class Vehicle;

class GameWorld
{
public:
	GameWorld();
	~GameWorld();

	void Update(double t);
	void Render();
	
private:
	Vehicle* mVehicle;
};
