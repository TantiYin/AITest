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
		mSide(mHeading.Perp()),
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

	bool      IsSpeedMaxedOut()const { return mMaxSpeed*mMaxSpeed >= mVelocity.LengthSq(); }
	double     Speed()const { return mVelocity.Length(); }
	double     SpeedSq()const { return mVelocity.LengthSq(); }

	double     MaxForce()const { return mMaxForce; }
	void      SetMaxForce(double mf) { mMaxForce = mf; }

	Vector2  Heading()const { return mHeading; }
	void      SetHeading(Vector2 new_heading);

	Vector2  Side()const { return mSide; }

protected:
	Vector2 mVelocity;//speed
	Vector2 mHeading; //heading
	Vector2 mSide;	//side perpendicular to heading

	double mMass;	//mass
	double mMaxSpeed;	//max speed
	double mMaxForce;	//max force

};



