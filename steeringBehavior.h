#pragma once


#include "vector2d.h"
#include <vector>
#include "baseEntity.h"
#include "wall2d.h"

class Vehicle;

//--------------------------- Constants ----------------------------------

//the radius of the constraining circle for the wander behavior
const double WanderRad = 15;
//distance the wander circle is projected in front of the agent
const double WanderDist = 10;
//the maximum amount of displacement along the circle each frame
const double WanderJitterPerSec = 180.0;
//the min length of detection box
const double DetectionLength = 20;
//the wall detection length
const double WallDetectionLength = 20;



class SteeringBehavior
{
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

    //the current position on the wander circle the agent is
    //attempting to steer towards
    Vector2     m_vWanderTarget;

    //explained above
    double        m_dWanderJitter;
    double        m_dWanderRadius;
    double        m_dWanderDistance;

    //length of the 'detection box' utilized in obstacle avoidance
    double                 m_dDBoxLength;
    //this will keep track of the closest intersecting obstacle (CIB)
    BaseGameEntity* ClosestIntersectingObstacle = NULL;
    //this will record the transformed local coordinates of the CIB
    Vector2 LocalPosOfClosestObstacle;

    //a vertex buffer to contain the feelers rqd for wall avoidance  
    std::vector<Vector2> m_Feelers;

    //the length of the 'feeler/s' used in wall detection
    double                 m_dWallDetectionFeelerLength;

private:

    //this function tests if a specific bit of m_iFlags is set
    bool      On(behavior_type bt){ return (m_iFlags & bt) == bt; }

    bool      AccumulateForce(Vector2 &RunningTot, Vector2 ForceToAdd);

    //creates the antenna utilized by the wall avoidance behavior
    void      CreateFeelers();

    /* .......................................................

    BEGIN BEHAVIOR DECLARATIONS

    .......................................................*/
    //this behavior moves the agent towards a target position
    Vector2 Seek(Vector2 TargetPos);

    //this behavior is similar to seek but it attempts to arrive 
    //at the target position with a zero velocity
    Vector2 Arrive(Vector2 TargetPos);

    //this behavior makes the agent wander about randomly
    Vector2 Wander();


    //this returns a steering force which will attempt to keep the agent 
    //away from any obstacles it may encounter
    Vector2 ObstacleAvoidance(const std::vector<BaseGameEntity*>& obstacles);

    //this returns a steering force which will keep the agent away from any
    //walls it may encounter
    Vector2 WallAvoidance(const std::vector<Wall2d> &walls);

public:
	SteeringBehavior(Vehicle * target);
	virtual ~SteeringBehavior(){}

	void Render();

	Vector2 Calculate();

	void SetTargetPos(const Vector2& TargetPos) {mTargetPos = TargetPos;}
	Vector2 GetTargetPos() { return mTargetPos; }

	void FleeOn() { m_iFlags |= flee; }
	void SeekOn() { m_iFlags |= seek; }
	void ArriveOn() { m_iFlags |= arrive; }
	void WanderOn() { m_iFlags |= wander; }
	void ObstacleAvoidanceOn() { m_iFlags |= obstacle_avoidance; }
    void WallAvoidanceOn(){ m_iFlags |= wall_avoidance; }


	void FleeOff() { if (On(flee))   m_iFlags ^= flee; }
	void SeekOff() { if (On(seek))   m_iFlags ^= seek; }
	void ArriveOff() { if (On(arrive)) m_iFlags ^= arrive; }
	void WanderOff() { if (On(wander)) m_iFlags ^= wander; }
	void ObstacleAvoidanceOff() { if (On(obstacle_avoidance)) m_iFlags ^= obstacle_avoidance; }
    void WallAvoidanceOff(){ if (On(wall_avoidance)) m_iFlags ^= wall_avoidance; }

	//Vector2 GetWanderTargetPos() { return m_vWanderTarget; }
	double GetWanderRadius() { return m_dWanderRadius; }
	double GetWanderDist() { return m_dWanderDistance; }

	double GetDLength() { return m_dDBoxLength; }
	int GetClosetObstacleID() { return ClosestIntersectingObstacle ? ClosestIntersectingObstacle->GetID() : 0; }
	Vector2 GetClosetObstacleCenter() { return LocalPosOfClosestObstacle; }
	
};



