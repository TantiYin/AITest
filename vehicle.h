#pragma once


#include "movingEntity.h"
#include <vector>
class SteeringBehavior;

class Vehicle: public MovingEntity
{
public:
	Vehicle(int id, Vector2 pos, double r, Vector2 v, Vector2 head, double m, double maxspeed, double maxforce);
	
	~Vehicle() {}

	void Update(double t);
	bool HandleMessage(const Telegram&) { return true; }
	void Render();

private:
	//the steering behavior class
	SteeringBehavior*     mpSteering;

	std::vector<Vector2> mvecVehicleVB;
};

