#pragma once


#include "movingEntity.h"
#include <vector>
class SteeringBehavior;
class GameWorld;

class Vehicle: public MovingEntity
{
public:
	Vehicle(GameWorld* world, Vector2 pos, double r, Vector2 v, Vector2 head, double m, double maxspeed, double maxforce);
	
	~Vehicle() {}

	void Update(double t);
	bool HandleMessage(const Telegram&) { return true; }
	void Render();

	GameWorld* GetWorld() { return mWorld; }
private:
	GameWorld* mWorld;

	//the steering behavior class
	SteeringBehavior*     mpSteering;

	std::vector<Vector2> mvecVehicleVB;
};

