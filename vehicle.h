#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "movingEntity.h"
class SteeringBehavior;

class Vehicle:public MovingEntity
{
public:
	Vehicle(int id, Vector2 pos, double r, Vector2 v, Vector2 head, double m, double maxspeed, double maxforce);
	
	~Vehicle(){}

	void Update(double t);
	bool HandleMessage(const Telegram&){ return true; }
private:
	//the steering behavior class
	SteeringBehavior*     m_pSteering;

	
};

#endif