#ifndef _MOVING_ENTITY_H_
#define _MOVING_ENTITY_H_

#include "npc.h"

class MovingEntity :public BaseGameEntity
{
public:
	MovingEntity(int id, Vector2 pos, double r, Vector2 v, Vector2 head, double m, double maxspeed, double maxforce) :
		BaseGameEntity(id, pos, r),
		mVelocity(v),
		mHeading(head),
		mMass(m),
		mMaxSpeed(maxspeed),
		mMaxForce(maxforce)
	{}
	~MovingEntity(){}

	//accessors
	Vector2  Velocity()const{ return mVelocity; }
	void      SetVelocity(const Vector2& NewVel){ mVelocity = NewVel; }

	double     MaxSpeed()const{ return mMaxSpeed; }
	void      SetMaxSpeed(double new_speed){ mMaxSpeed = new_speed; }
protected:
	Vector2 mVelocity;//speed
	Vector2 mHeading; //heading
	//Vector2 mSide;	//side perpendicular to heading

	double mMass;	//mass
	double mMaxSpeed;	//max speed
	double mMaxForce;	//max force

};

#endif