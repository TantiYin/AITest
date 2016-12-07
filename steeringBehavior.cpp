#include "steeringBehavior.h"
#include "vehicle.h"
#include "GameWorld.h"
#include <algorithm>
#include "utils.h"
#include "transformation.h"

SteeringBehavior::SteeringBehavior(Vehicle * target) :
m_pVehicle(target),
m_iFlags(0),
m_vSteeringForce(Vector2(0,0)),
mTargetPos(Vector2(0, 0)),
m_dWanderDistance(WanderDist),
m_dWanderJitter(WanderJitterPerSec),
m_dWanderRadius(WanderRad)

{

}

Vector2 SteeringBehavior::Seek(Vector2 TargetPos)
{
	Vector2 desiredVelocity = Vec2DNormalize(TargetPos - m_pVehicle->Pos()) * m_pVehicle->MaxSpeed();
	return desiredVelocity - m_pVehicle->Velocity();
}


Vector2 SteeringBehavior::Arrive(Vector2 TargetPos)
{
	Vector2 ToTarget = TargetPos - m_pVehicle->Pos();

	//calculate the distance to the target
	double dist = ToTarget.Length();

	if (dist > 0)
	{
		//calculate the speed required to reach the target given the desired
		//deceleration
		double speed = dist;

		//make sure the velocity does not exceed the max
		speed = (std::min)(speed, m_pVehicle->MaxSpeed());

		//from here proceed just like Seek except we don't need to normalize 
		//the ToTarget vector because we have already gone to the trouble
		//of calculating its length: dist. 
		Vector2 DesiredVelocity = ToTarget * speed / dist;

		return (DesiredVelocity - m_pVehicle->Velocity());
	}

	return Vector2(0, 0);
}


Vector2 SteeringBehavior::Wander()
{
	//this behavior is dependent on the update rate, so this line must
	//be included when using time independent framerate.
	double JitterThisTimeSlice = m_dWanderJitter * m_pVehicle->TimeElapsed();

	//first, add a small random vector to the target's position
	m_vWanderTarget += Vector2(RandomClamped() * JitterThisTimeSlice,
		RandomClamped() * JitterThisTimeSlice);

	//reproject this new vector back on to a unit circle
	m_vWanderTarget.Normalize();

	//increase the length of the vector to the same as the radius
	//of the wander circle
	m_vWanderTarget *= m_dWanderRadius;

	//move the target into a position WanderDist in front of the agent
	Vector2 target = m_vWanderTarget + Vector2(m_dWanderDistance, 0);

	//project the target into world space
	Vector2 Target = PointToWorldSpace(target,
		m_pVehicle->Heading(),
		m_pVehicle->Side(),
		m_pVehicle->Pos());

	mTargetPos = Target;
	//and steer towards it
	return Target - m_pVehicle->Pos();
}

Vector2 SteeringBehavior::Calculate()
{
	m_vSteeringForce.Zero();
	if (On(seek))
	{
		m_vSteeringForce += Seek(m_pVehicle->GetWorld()->Crosshair()); //* m_dWeightSeek;
	}

	if (On(arrive))
	{
		m_vSteeringForce += Arrive(m_pVehicle->GetWorld()->Crosshair());
	}

	if (On(wander))
	{
		m_vSteeringForce += Wander();
	}
	return m_vSteeringForce;
}
