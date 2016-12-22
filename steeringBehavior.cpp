#include "steeringBehavior.h"
#include "vehicle.h"
#include "GameWorld.h"
#include <algorithm>
#include "utils.h"
#include "transformation.h"
#include <d2d1.h>
#include <string>
#include <sstream>
#include "ParamLoader.h"

extern ID2D1HwndRenderTarget* gpRenderTarget;
extern ID2D1SolidColorBrush* gpBrush;
extern IDWriteTextFormat* gpTextFormat;

SteeringBehavior::SteeringBehavior(Vehicle * target) :
m_pVehicle(target),
m_iFlags(0),
m_vSteeringForce(Vector2(0,0)),
mTargetPos(Vector2(0, 0)),
m_dWanderDistance(WanderDist),
m_dWanderJitter(WanderJitterPerSec),
m_dWanderRadius(WanderRad),
m_dDBoxLength(Prm.MinDetectionBoxLength),
m_dViewDistance(Prm.ViewDistance),
m_Feelers(3),
m_dWallDetectionFeelerLength(Prm.WallDetectionFeelerLength),
m_dWaypointSeekDistSq(WaypointSeekDist*WaypointSeekDist),
m_dWeightSeek(Prm.SeekWeight),
m_dWeightFlee(Prm.FleeWeight),
m_dWeightArrive(Prm.ArriveWeight),
m_dWeightWander(Prm.WanderWeight),
m_dWeightPursuit(Prm.PursuitWeight),
m_dWeightOffsetPursuit(Prm.OffsetPursuitWeight),
m_dWeightInterpose(Prm.InterposeWeight),
m_dWeightHide(Prm.HideWeight),
m_dWeightEvade(Prm.EvadeWeight),
m_dWeightFollowPath(Prm.FollowPathWeight),
m_dWeightCohesion(Prm.CohesionWeight),
m_dWeightAlignment(Prm.AlignmentWeight),
m_dWeightSeparation(Prm.SeparationWeight),
m_dWeightObstacleAvoidance(Prm.ObstacleAvoidanceWeight),
m_dWeightWallAvoidance(Prm.WallAvoidanceWeight)
{
	//create a Path
	m_pPath = new Path();
	m_pPath->LoopOn();
}


bool SteeringBehavior::AccumulateForce(Vector2 &RunningTot, Vector2 ForceToAdd)
{

	//calculate how much steering force the vehicle has used so far
	double MagnitudeSoFar = RunningTot.Length();

	//calculate how much steering force remains to be used by this vehicle
	double MagnitudeRemaining = m_pVehicle->MaxForce() - MagnitudeSoFar;

	//return false if there is no more force left to use
	if (MagnitudeRemaining <= 0.0) return false;

	//calculate the magnitude of the force we want to add
	double MagnitudeToAdd = ForceToAdd.Length();

	//if the magnitude of the sum of ForceToAdd and the running total
	//does not exceed the maximum force available to this vehicle, just
	//add together. Otherwise add as much of the ForceToAdd vector is
	//possible without going over the max.
	if (MagnitudeToAdd < MagnitudeRemaining)
	{
		RunningTot += ForceToAdd;
	}

	else
	{
		//add it to the steering force
		RunningTot += (Vec2DNormalize(ForceToAdd) * MagnitudeRemaining);
	}

	return true;
}

Vector2 SteeringBehavior::Seek(Vector2 TargetPos)
{
	Vector2 desiredVelocity = Vec2DNormalize(TargetPos - m_pVehicle->Pos()) * m_pVehicle->MaxSpeed();
	return desiredVelocity - m_pVehicle->Velocity();
}


Vector2 SteeringBehavior::Flee(Vector2 TargetPos)
{
	//only flee if the target is within 'panic distance'. Work in distance
	//squared space.
	/* const double PanicDistanceSq = 100.0f * 100.0;
	if (Vec2DDistanceSq(m_pVehicle->Pos(), target) > PanicDistanceSq)
	{
	return Vector2(0,0);
	}
	*/

	Vector2 DesiredVelocity = Vec2DNormalize(m_pVehicle->Pos() - TargetPos)
		* m_pVehicle->MaxSpeed();

	return (DesiredVelocity - m_pVehicle->Velocity());
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


Vector2 SteeringBehavior::Evade(const Vehicle* pursuer)
{
	/* Not necessary to include the check for facing direction this time */

	Vector2 ToPursuer = pursuer->Pos() - m_pVehicle->Pos();

	//uncomment the following two lines to have Evade only consider pursuers 
	//within a 'threat range'
	const double ThreatRange = 100.0;
	if (ToPursuer.LengthSq() > ThreatRange * ThreatRange) return Vector2();

	//the lookahead time is propotional to the distance between the pursuer
	//and the pursuer; and is inversely proportional to the sum of the
	//agents' velocities
	double LookAheadTime = ToPursuer.Length() /
		(m_pVehicle->MaxSpeed() + pursuer->Speed());

	//now flee away from predicted future position of the pursuer
	return Flee(pursuer->Pos() + pursuer->Velocity() * LookAheadTime);
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


Vector2 SteeringBehavior::ObstacleAvoidance(const std::vector<BaseGameEntity*>& obstacles)
{
	//the detection box length is proportional to the agent's velocity
	m_dDBoxLength = Prm.MinDetectionBoxLength +
		(m_pVehicle->Speed() / m_pVehicle->MaxSpeed()) *
		Prm.MinDetectionBoxLength;

	//tag all obstacles within range of the box for processing
	m_pVehicle->GetWorld()->TagObstaclesWithinViewRange(m_pVehicle, m_dDBoxLength);

	//this will keep track of the closest intersecting obstacle (CIB)
	ClosestIntersectingObstacle = NULL;

	//this will be used to track the distance to the CIB
	double DistToClosestIP = MaxDouble;

	//this will record the transformed local coordinates of the CIB
	LocalPosOfClosestObstacle = Vector2(0, 0);

	std::vector<BaseGameEntity*>::const_iterator curOb = obstacles.begin();

	while (curOb != obstacles.end())
	{
		//if the obstacle has been tagged within range proceed
		if ((*curOb)->IsTagged())
		{
			//calculate this obstacle's position in local space
			Vector2 LocalPos = PointToLocalSpace((*curOb)->Pos(),
				m_pVehicle->Heading(),
				m_pVehicle->Side(),
				m_pVehicle->Pos());

			//if the local position has a negative x value then it must lay
			//behind the agent. (in which case it can be ignored)
			if (LocalPos.x >= 0)
			{
				//if the distance from the x axis to the object's position is less
				//than its radius + half the width of the detection box then there
				//is a potential intersection.
				double ExpandedRadius = (*curOb)->BRadius() + m_pVehicle->BRadius();

				if (fabs(LocalPos.y) < ExpandedRadius)
				{
					//now to do a line/circle intersection test. The center of the 
					//circle is represented by (cX, cY). The intersection points are 
					//given by the formula x = cX +/-sqrt(r^2-cY^2) for y=0. 
					//We only need to look at the smallest positive value of x because
					//that will be the closest point of intersection.
					double cX = LocalPos.x;
					double cY = LocalPos.y;

					//we only need to calculate the sqrt part of the above equation once
					double SqrtPart = sqrt(ExpandedRadius*ExpandedRadius - cY*cY);

					double ip = cX - SqrtPart;

					if (ip <= 0.0)
					{
						ip = cX + SqrtPart;
					}

					//test to see if this is the closest so far. If it is keep a
					//record of the obstacle and its local coordinates
					if (ip < DistToClosestIP)
					{
						DistToClosestIP = ip;

						ClosestIntersectingObstacle = *curOb;

						LocalPosOfClosestObstacle = LocalPos;
					}
				}
			}
		}

		++curOb;
	}

	//if we have found an intersecting obstacle, calculate a steering 
	//force away from it
	Vector2 SteeringForce;

	if (ClosestIntersectingObstacle)
	{
		//the closer the agent is to an object, the stronger the 
		//steering force should be
		double multiplier = 1.0 + (m_dDBoxLength - LocalPosOfClosestObstacle.x) /
			m_dDBoxLength;

		//calculate the lateral force
		SteeringForce.y = (ClosestIntersectingObstacle->BRadius() + m_pVehicle->BRadius() - fabs(LocalPosOfClosestObstacle.y))  * multiplier;

		const double Threshold = 0;
		if (fabs(LocalPosOfClosestObstacle.y) > Threshold)
		{
			LocalPosOfClosestObstacle.y > 0 ? SteeringForce.y *= -1 : SteeringForce.y *= 1;
		}
		//apply a braking force proportional to the obstacles distance from
		//the vehicle. 
		const double BrakingWeight = 0.2;

		SteeringForce.x = (ClosestIntersectingObstacle->BRadius() -
			LocalPosOfClosestObstacle.x) *
			BrakingWeight;
	}

	//finally, convert the steering vector from local to world space
	return VectorToWorldSpace(SteeringForce,
		m_pVehicle->Heading(),
		m_pVehicle->Side());
}

//for receiving keyboard input from user
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)

void SteeringBehavior::Render()
{
	{
		if (KEYDOWN(VK_INSERT)) { m_pVehicle->SetMaxForce(m_pVehicle->MaxForce() + 1000.0f*m_pVehicle->TimeElapsed()); }
		if (KEYDOWN(VK_DELETE)) { if (m_pVehicle->MaxForce() > 0.2f) m_pVehicle->SetMaxForce(m_pVehicle->MaxForce() - 1000.0f*m_pVehicle->TimeElapsed()); }
		if (KEYDOWN(VK_HOME)) { m_pVehicle->SetMaxSpeed(m_pVehicle->MaxSpeed() + 50.0f*m_pVehicle->TimeElapsed()); }
		if (KEYDOWN(VK_END)) { if (m_pVehicle->MaxSpeed() > 0.2f) m_pVehicle->SetMaxSpeed(m_pVehicle->MaxSpeed() - 50.0f*m_pVehicle->TimeElapsed()); }

		if (m_pVehicle->MaxForce() < 0) m_pVehicle->SetMaxForce(0.0f);
		if (m_pVehicle->MaxSpeed() < 0) m_pVehicle->SetMaxSpeed(0.0f);

		std::wstringstream ss;
		std::wstring LineText;

		ss << L"MaxForce(Ins/Del):" << m_pVehicle->MaxForce();
		ss >> LineText;
		gpRenderTarget->DrawText(LineText.c_str(), LineText.size(), gpTextFormat, D2D1::Rect(0, 0, 300, 20), gpBrush);
		ss.clear();
		ss << L"MaxSpeed(Home/End):" << m_pVehicle->MaxSpeed();
		ss >> LineText;
		gpRenderTarget->DrawText(LineText.c_str(), LineText.size(), gpTextFormat, D2D1::Rect(0, 20, 300, 40), gpBrush);
	}


	if (false/*On(wander)*/)
	{
		if (KEYDOWN('F')) { m_dWanderJitter += 1.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderJitter, 0.0f, 100.0f); }
		if (KEYDOWN('V')) { m_dWanderJitter -= 1.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderJitter, 0.0f, 100.0f); }
		if (KEYDOWN('G')) { m_dWanderDistance += 2.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderDistance, 0.0f, 50.0f); }
		if (KEYDOWN('B')) { m_dWanderDistance -= 2.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderDistance, 0.0f, 50.0f); }
		if (KEYDOWN('H')) { m_dWanderRadius += 2.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderRadius, 0.0f, 100.0f); }
		if (KEYDOWN('N')) { m_dWanderRadius -= 2.0f*m_pVehicle->TimeElapsed(); Clamp(m_dWanderRadius, 0.0f, 100.0f); }

		std::wstringstream ss;
		std::wstring LineText;
		ss << L"Jitter(F/V):" << m_dWanderJitter;
		ss >> LineText;
		gpRenderTarget->DrawText(LineText.c_str(), LineText.size(), gpTextFormat, D2D1::Rect(0, 40, 300, 60), gpBrush);
		ss.clear();
		ss << L"Distance(G/B):" << m_dWanderDistance;
		ss >> LineText;
		gpRenderTarget->DrawText(LineText.c_str(), LineText.size(), gpTextFormat, D2D1::Rect(0, 60, 300, 80), gpBrush);
		ss.clear();
		ss << L"Radius(H/N):" << m_dWanderRadius;
		ss >> LineText;
		gpRenderTarget->DrawText(LineText.c_str(), LineText.size(), gpTextFormat, D2D1::Rect(0, 80, 300, 100), gpBrush);

		//wander»¶
		Vector2 CircleCenter = Vector2(GetWanderDist(), 0);
		CircleCenter = PointToWorldSpace(CircleCenter, m_pVehicle->Heading(), m_pVehicle->Side(), m_pVehicle->Pos());
		D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2(CircleCenter.x, CircleCenter.y), GetWanderRadius(), GetWanderRadius());
		gpBrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 1.0f));
		gpRenderTarget->DrawEllipse(ellipse, gpBrush, 1);

		//ƒø±Í
		ellipse = D2D1::Ellipse(D2D1::Point2(GetTargetPos().x, GetTargetPos().y), 2, 2);
		gpBrush->SetColor(D2D1::ColorF(1.0f, 0.0f, 0.0f));
		gpRenderTarget->DrawEllipse(ellipse, gpBrush, 1);
	}

	if (On(obstacle_avoidance))
	{
		//ºÏ≤‚∫–
		std::vector<Vector2> DBoxVerticle;
		DBoxVerticle.push_back(Vector2(0, -1 * m_pVehicle->BRadius()));
		DBoxVerticle.push_back(Vector2(GetDLength(), -1 * m_pVehicle->BRadius()));
		DBoxVerticle.push_back(Vector2(GetDLength(), m_pVehicle->BRadius()));
		DBoxVerticle.push_back(Vector2(0, m_pVehicle->BRadius()));

		std::vector<Vector2> DBoxVerticleTrans = WorldTransform(DBoxVerticle, m_pVehicle->Pos(), m_pVehicle->Heading(), m_pVehicle->Side());
		int VecNum = DBoxVerticleTrans.size();
		for (int i = 0; i < VecNum - 1; ++i)
		{
			gpRenderTarget->DrawLine(D2D1::Point2(DBoxVerticleTrans[i].x, DBoxVerticleTrans[i].y)
				, D2D1::Point2(DBoxVerticleTrans[i + 1].x, DBoxVerticleTrans[i + 1].y)
				, gpBrush, 1);
		}
		gpRenderTarget->DrawLine(D2D1::Point2(DBoxVerticleTrans[VecNum - 1].x, DBoxVerticleTrans[VecNum - 1].y)
			, D2D1::Point2(DBoxVerticleTrans[0].x, DBoxVerticleTrans[0].y)
			, gpBrush, 1);

		//’œ∞≠ŒÔ
		std::wstringstream ss;
		std::wstring LineText;
		ss << L"closetObstacleID:" << GetClosetObstacleID();
		ss >> LineText;
		gpRenderTarget->DrawText(LineText.c_str(), LineText.size(), gpTextFormat
			, D2D1::Rect(m_pVehicle->GetWorld()->cxClient() - 300, 0, m_pVehicle->GetWorld()->cxClient(), 20)
			, gpBrush);
		ss.clear();
		ss << L"closetObstacleCenter:" << static_cast<int>(GetClosetObstacleCenter().x) << L"," << static_cast<int>(GetClosetObstacleCenter().y);
		ss >> LineText;
		gpRenderTarget->DrawText(LineText.c_str(), LineText.size(), gpTextFormat
			, D2D1::Rect(m_pVehicle->GetWorld()->cxClient() - 300, 20, m_pVehicle->GetWorld()->cxClient(), 40)
			, gpBrush);
	}

    if (On(wall_avoidance))
    {
        for (int i = 0; i < m_Feelers.size(); ++i)
        {
            gpRenderTarget->DrawLine(D2D1::Point2(m_pVehicle->Pos().x, m_pVehicle->Pos().y), D2D1::Point2(m_Feelers[i].x, m_Feelers[i].y), gpBrush, 1);
        }
    }
}

Vector2 SteeringBehavior::Calculate()
{
	m_vSteeringForce.Zero();
	Vector2 force;

	if (On(cohesion) || On(separation) || On(allignment))
	{
		m_pVehicle->GetWorld()->TagVehiclesWithinViewRange(m_pVehicle, m_dViewDistance);
	}

    if (On(wall_avoidance))
    {
		force = WallAvoidance(m_pVehicle->GetWorld()->Walls()) * m_dWeightWallAvoidance;
        if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
    }

	if (On(obstacle_avoidance))
	{
		force = ObstacleAvoidance(m_pVehicle->GetWorld()->Obstacles()) * m_dWeightObstacleAvoidance;
		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(separation))
	{
		force = Separation(m_pVehicle->GetWorld()->Agents()) * m_dWeightSeparation;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(allignment))
	{
		force = Alignment(m_pVehicle->GetWorld()->Agents()) * m_dWeightAlignment;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(cohesion))
	{
		force = Cohesion(m_pVehicle->GetWorld()->Agents()) * m_dWeightCohesion;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(seek))
	{
		force = Seek(m_pVehicle->GetWorld()->Crosshair()) * m_dWeightSeek;
		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(arrive))
	{
		force = Arrive(m_pVehicle->GetWorld()->Crosshair()) * m_dWeightArrive;
		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(wander))
	{
		force = Wander() * m_dWeightWander;
		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(hide))
	{
		force = Hide(m_pTargetAgent1, m_pVehicle->GetWorld()->Obstacles()) * m_dWeightHide;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	if (On(follow_path))
	{
		force = FollowPath() * m_dWeightFollowPath;

		if (!AccumulateForce(m_vSteeringForce, force)) return m_vSteeringForce;
	}

	return m_vSteeringForce;
}

Vector2 SteeringBehavior::WallAvoidance(const std::vector<Wall2d> &walls)
{
    //the feelers are contained in a std::vector, m_Feelers
    CreateFeelers();

    double DistToThisIP = 0.0;
    double DistToClosestIP = MaxDouble;

    //this will hold an index into the vector of walls
    int ClosestWall = -1;

    Vector2 SteeringForce,
        point,         //used for storing temporary info
        ClosestPoint;  //holds the closest intersection point

    //examine each feeler in turn
    for (unsigned int flr = 0; flr<m_Feelers.size(); ++flr)
    {
        //run through each wall checking for any intersection points
        for (unsigned int w = 0; w<walls.size(); ++w)
        {
            if (LineIntersection2D(m_pVehicle->Pos(),
                m_Feelers[flr],
                walls[w].From(),
                walls[w].To(),
                DistToThisIP,
                point))
            {
                //is this the closest found so far? If so keep a record
                if (DistToThisIP < DistToClosestIP)
                {
                    DistToClosestIP = DistToThisIP;

                    ClosestWall = w;

                    ClosestPoint = point;
                }
            }
        }//next wall


        //if an intersection point has been detected, calculate a force  
        //that will direct the agent away
        if (ClosestWall >= 0)
        {
            //calculate by what distance the projected position of the agent
            //will overshoot the wall
            Vector2 OverShoot = m_Feelers[flr] - ClosestPoint;

            //create a force in the direction of the wall normal, with a 
            //magnitude of the overshoot
            SteeringForce = walls[ClosestWall].Normal() * OverShoot.Length();
        }

    }//next feeler

    return SteeringForce;

}

Vector2 SteeringBehavior::FollowPath()
{
	//move to next target if close enough to current target (working in
	//distance squared space)
	if (Vec2DDistanceSq(m_pPath->CurrentWaypoint(), m_pVehicle->Pos()) <
		m_dWaypointSeekDistSq)
	{
		m_pPath->SetNextWaypoint();
	}

	if (!m_pPath->Finished())
	{
		return Seek(m_pPath->CurrentWaypoint());
	}

	else
	{
		return Arrive(m_pPath->CurrentWaypoint());
	}
}

Vector2 SteeringBehavior::Hide(const Vehicle* hunter, const std::vector<BaseGameEntity*>& obstacles)
{
	double    DistToClosest = MaxDouble;
	Vector2 BestHidingSpot;

	std::vector<BaseGameEntity*>::const_iterator curOb = obstacles.begin();
	std::vector<BaseGameEntity*>::const_iterator closest;

	while (curOb != obstacles.end())
	{
		//calculate the position of the hiding spot for this obstacle
		Vector2 HidingSpot = GetHidingPosition((*curOb)->Pos(),
			(*curOb)->BRadius(),
			hunter->Pos());

		//work in distance-squared space to find the closest hiding
		//spot to the agent
		double dist = Vec2DDistanceSq(HidingSpot, m_pVehicle->Pos());

		if (dist < DistToClosest)
		{
			DistToClosest = dist;

			BestHidingSpot = HidingSpot;

			closest = curOb;
		}

		++curOb;

	}//end while

	 //if no suitable obstacles found then Evade the hunter
	if (DistToClosest == MaxFloat)
	{
		return Evade(hunter);
	}

	//else use Arrive on the hiding spot
	return Arrive(BestHidingSpot);
}

Vector2 SteeringBehavior::Cohesion(const std::vector<Vehicle*>& neighbors)
{
	//first find the center of mass of all the agents
	Vector2 CenterOfMass, SteeringForce;

	int NeighborCount = 0;

	//iterate through the neighbors and sum up all the position vectors
	for (unsigned int a = 0; a < neighbors.size(); ++a)
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined is close enough ***also make sure it doesn't
		//include the evade target ***
		if ((neighbors[a] != m_pVehicle) && neighbors[a]->IsTagged()/* &&
			(neighbors[a] != m_pTargetAgent1)*/)
		{
			CenterOfMass += neighbors[a]->Pos();

			++NeighborCount;
		}
	}

	if (NeighborCount > 0)
	{
		//the center of mass is the average of the sum of positions
		CenterOfMass /= (double)NeighborCount;

		//now seek towards that position
		SteeringForce = Seek(CenterOfMass);
	}

	//the magnitude of cohesion is usually much larger than separation or
	//allignment so it usually helps to normalize it.
	return Vec2DNormalize(SteeringForce);

}

Vector2 SteeringBehavior::Separation(const std::vector<Vehicle*>& neighbors)
{
	Vector2 SteeringForce;

	for (unsigned int a = 0; a < neighbors.size(); ++a)
	{
		//make sure this agent isn't included in the calculations and that
		//the agent being examined is close enough. ***also make sure it doesn't
		//include the evade target ***
		if ((neighbors[a] != m_pVehicle) && neighbors[a]->IsTagged()/* &&
			(neighbors[a] != m_pTargetAgent1)*/)
		{
			Vector2 ToAgent = m_pVehicle->Pos() - neighbors[a]->Pos();

			//scale the force inversely proportional to the agents distance  
			//from its neighbor.
			SteeringForce += Vec2DNormalize(ToAgent) / ToAgent.Length();
		}
	}

	return SteeringForce;

}

Vector2 SteeringBehavior::Alignment(const std::vector<Vehicle*>& neighbors)
{
	//used to record the average heading of the neighbors
	Vector2 AverageHeading;

	//used to count the number of vehicles in the neighborhood
	int    NeighborCount = 0;

	//iterate through all the tagged vehicles and sum their heading vectors  
	for (unsigned int a = 0; a < neighbors.size(); ++a)
	{
		//make sure *this* agent isn't included in the calculations and that
		//the agent being examined  is close enough ***also make sure it doesn't
		//include any evade target ***
		if ((neighbors[a] != m_pVehicle) && neighbors[a]->IsTagged()/* &&
			(neighbors[a] != m_pTargetAgent1)*/)
		{
			AverageHeading += neighbors[a]->Heading();

			++NeighborCount;
		}
	}

	//if the neighborhood contained one or more vehicles, average their
	//heading vectors.
	if (NeighborCount > 0)
	{
		AverageHeading /= (double)NeighborCount;

		AverageHeading -= m_pVehicle->Heading();
	}

	return AverageHeading;

}

Vector2 SteeringBehavior::GetHidingPosition(const Vector2& posOb, const double radiusOb, const Vector2& posHunter)
{
	//calculate how far away the agent is to be from the chosen obstacle's
	//bounding radius
	const double DistanceFromBoundary = 30.0;
	double       DistAway = radiusOb + DistanceFromBoundary;

	//calculate the heading toward the object from the hunter
	Vector2 ToOb = Vec2DNormalize(posOb - posHunter);

	//scale it to size and add to the obstacles position to get
	//the hiding spot.
	return (ToOb * DistAway) + posOb;
}

void SteeringBehavior::CreateFeelers()
{
    //feeler pointing straight in front
    m_Feelers[0] = m_pVehicle->Pos() + m_dWallDetectionFeelerLength * m_pVehicle->Heading();

    //feeler to left
    Vector2 temp = m_pVehicle->Heading();
    Vec2DRotateAroundOrigin(temp, HalfPi * 3.5f);
    m_Feelers[1] = m_pVehicle->Pos() + m_dWallDetectionFeelerLength / 2.0f * temp;

    //feeler to right
    temp = m_pVehicle->Heading();
    Vec2DRotateAroundOrigin(temp, HalfPi * 0.5f);
    m_Feelers[2] = m_pVehicle->Pos() + m_dWallDetectionFeelerLength / 2.0f * temp;
}
