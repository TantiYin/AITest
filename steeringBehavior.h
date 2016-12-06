#pragma once


#include "vector2d.h"

class Vehicle;

class SteeringBehavior
{
public:
	SteeringBehavior(Vehicle * target);
	~SteeringBehavior(){}

	Vector2 Calculate();

	void SetTargetPos(const Vector2& TargetPos) {mTargetPos = TargetPos;}
	Vector2 GetTargetPos() { return mTargetPos; }
	void SeekOn(){ m_iFlags |= seek; }
	/* .......................................................

	BEGIN BEHAVIOR DECLARATIONS

	.......................................................*/
	//this behavior moves the agent towards a target position
	Vector2 Seek(Vector2 TargetPos);
private:
	enum behavior_type
	{
		none = 0x00000,
		seek = 0x00002,
		flee = 0x00004,
		arrive = 0x00008,
		wander = 0x00010,
		cohesion = 0x00020,
		separation = 0x00040,
		allignment = 0x00080,
		obstacle_avoidance = 0x00100,
		wall_avoidance = 0x00200,
		follow_path = 0x00400,
		pursuit = 0x00800,
		evade = 0x01000,
		interpose = 0x02000,
		hide = 0x04000,
		flock = 0x08000,
		offset_pursuit = 0x10000,
	};
private:
	//a pointer to the owner of this instance
	Vehicle*     m_pVehicle;

	//∫œ¡¶
	//the steering force created by the combined effect of all
	//the selected behaviors
	Vector2    m_vSteeringForce;

	//binary flags to indicate whether or not a behavior should be active
	int           m_iFlags;

	Vehicle* mSeekTarget;
	Vehicle* mPursuitTarget;
	Vehicle* mEvadeTarget;

	Vector2 mTargetPos;

	//this function tests if a specific bit of m_iFlags is set
	bool      On(behavior_type bt){ return (m_iFlags & bt) == bt; }
};



