#include "vehicle.h"
#include "steeringBehavior.h"

Vehicle::Vehicle(int id, Vector2 pos, double r, Vector2 v, Vector2 head, double m, double maxspeed, double maxforce):
MovingEntity(id, pos, r, v, head, m, maxspeed, maxforce)
{
	//set up the steering behavior class
	m_pSteering = new SteeringBehavior(this);
	m_pSteering->SeekOn();
}

void Vehicle::Update(double t)
{

	//keep a record of its old position so we can update its cell later
	//in this method
	Vector2 OldPos = Pos();


	Vector2 SteeringForce;

	//calculate the combined force from each steering behavior in the 
	//vehicle's list
	SteeringForce = m_pSteering->Calculate();

	//Acceleration = Force/Mass
	Vector2 acceleration = SteeringForce / mMass;

	//update velocity
	mVelocity += acceleration * t;

	//make sure vehicle does not exceed maximum velocity
	mVelocity.Truncate(mMaxSpeed);

	//update the position
	mPos += mVelocity * t;
}