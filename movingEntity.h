#pragma once


#include "baseEntity.h"
#include "entityName.h"

class MovingEntity: public BaseGameEntity
{
public:
	MovingEntity(Vector2 pos, double r, Vector2 v, Vector2 head, double m, double maxspeed, double maxforce) :
		BaseGameEntity(ent_Bus, pos, r),
		mVelocity(v),
		mHeading(head),
		mMass(m),
		mMaxSpeed(maxspeed),
		mMaxForce(maxforce)
	{}
	~MovingEntity() {}

	//accessors
	Vector2  Velocity()const { return mVelocity; }
	void      SetVelocity(const Vector2& NewVel) { mVelocity = NewVel; }

	double     MaxSpeed()const { return mMaxSpeed; }
	void      SetMaxSpeed(double newSpeed) { mMaxSpeed = newSpeed; }
protected:
	Vector2 mVelocity;//speed
	Vector2 mHeading; //heading
	Vector2 mSide;	//side perpendicular to heading

	double mMass;	//mass
	double mMaxSpeed;	//max speed
	double mMaxForce;	//max force

};


