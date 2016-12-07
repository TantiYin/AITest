#include "steeringBehavior.h"
#include "vehicle.h"
#include "GameWorld.h"
#include <algorithm>

SteeringBehavior::SteeringBehavior(Vehicle * target) :
m_pVehicle(target),
m_iFlags(0),
m_vSteeringForce(Vector2(0,0)),
mTargetPos(Vector2(0, 0))

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
	return m_vSteeringForce;
}
