#include "steeringBehavior.h"
#include "vehicle.h"

SteeringBehavior::SteeringBehavior(Vehicle * target) :
m_pVehicle(target),
m_iFlags(0),
m_vSteeringForce(Vector2(0,0))

{

}

Vector2 SteeringBehavior::Seek(Vector2 TargetPos)
{
	Vector2 desiredVelocity = Vec2DNormalize(TargetPos - m_pVehicle->Pos()) * m_pVehicle->MaxSpeed();
	return desiredVelocity - m_pVehicle->Velocity();
}

Vector2 SteeringBehavior::Calculate()
{
	m_vSteeringForce.Zero();
	if (On(seek))
	{
		m_vSteeringForce += Seek(Vector2(10,100)); //* m_dWeightSeek;
	}
	return m_vSteeringForce;
}