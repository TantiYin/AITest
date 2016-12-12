#pragma once


#include "movingEntity.h"
#include <vector>
class SteeringBehavior;
class GameWorld;

class Vehicle: public MovingEntity
{
private:
	GameWorld* mWorld;

	//the steering behavior class
	SteeringBehavior*     mpSteering;

	//keeps a track of the most recent update time. (some of the
	//steering behaviors make use of this - see Wander)
	double                m_dTimeElapsed;

	std::vector<Vector2> mvecVehicleVB;

public:
	Vehicle(GameWorld* world, Vector2 pos, double r, Vector2 v, Vector2 head, double m, double maxspeed, double maxforce);
	
	~Vehicle() {}

	void Update(double t);
	bool HandleMessage(const Telegram&) { return true; }
	void Render();

	//-------------------------------------------accessor methods
	GameWorld* GetWorld() { return mWorld; }
	SteeringBehavior*const  Steering()const { return mpSteering; }
	double       TimeElapsed()const { return m_dTimeElapsed; }
};

