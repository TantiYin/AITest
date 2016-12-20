#include "vehicle.h"
#include "steeringBehavior.h"
#include "d2d1.h"
#include "transformation.h"
#include <string>
#include <sstream>
#include "GameWorld.h"

extern ID2D1HwndRenderTarget* gpRenderTarget;
extern ID2D1SolidColorBrush* gpBrush;
extern IDWriteTextFormat* gpTextFormat;

Vehicle::Vehicle(GameWorld* world, Vector2 pos, double r, Vector2 v, Vector2 head, double m, double maxspeed, double maxforce):
MovingEntity(pos, r, v, head, m, maxspeed, maxforce), mWorld(world), m_dTimeElapsed(0)
{
	//set up the steering behavior class
	mpSteering = new SteeringBehavior(this);
	//mpSteering->SeekOn();
	//mpSteering->ArriveOn();
	//mpSteering->WanderOn();
	//mpSteering->ObstacleAvoidanceOn();
    //mpSteering->WallAvoidanceOn();
	//mpSteering->FollowPathOn();

	const int NumVehicleVerts = 3;

	Vector2 vehicle[NumVehicleVerts] = { Vector2(-10.0f, r),
		Vector2(10.0f, 0.0f),
		Vector2(-10.0f, -r) };

	//setup the vertex buffers and calculate the bounding radius
	for (int vtx = 0; vtx < NumVehicleVerts; ++vtx)
	{
		mvecVehicleVB.push_back(vehicle[vtx]);
	}
}

void Vehicle::Update(double t)
{
	m_dTimeElapsed = t;

	//keep a record of its old position so we can update its cell later
	//in this method
	Vector2 OldPos = Pos();
	/*Vector2 TargetPos = Vector2(50, 100);
	mpSteering->SetTargetPos(TargetPos);*/

	Vector2 SteeringForce;

	//calculate the combined force from each steering behavior in the 
	//vehicle's list
	SteeringForce = mpSteering->Calculate();

	//Acceleration = Force/Mass
	Vector2 acceleration = SteeringForce / mMass;

	//update velocity
	mVelocity += acceleration * t;

	//make sure vehicle does not exceed maximum velocity
	mVelocity.Truncate(mMaxSpeed);

	//update the position
	mPos += mVelocity * t;

	//update the heading if the vehicle has a non zero velocity
	if (mVelocity.LengthSq() > 0.00000001)
	{
		mHeading = Vec2DNormalize(mVelocity);

		mSide = mHeading.Perp();
	}

	WrapAround(mPos, mWorld->cxClient(), mWorld->cyClient());

}

void Vehicle::Render()
{
	//图形
	static std::vector<Vector2>  m_vecVehicleVBTrans;

	m_vecVehicleVBTrans = WorldTransform(mvecVehicleVB, mPos, mHeading, mSide);
	gpBrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 1.0f));

	int VecNum = m_vecVehicleVBTrans.size();
	for (int i = 0; i < VecNum - 1; ++i)
	{
		gpRenderTarget->DrawLine(D2D1::Point2(m_vecVehicleVBTrans[i].x, m_vecVehicleVBTrans[i].y)
			, D2D1::Point2(m_vecVehicleVBTrans[i+1].x, m_vecVehicleVBTrans[i+1].y)
			, gpBrush, 1);
	}
	gpRenderTarget->DrawLine(D2D1::Point2(m_vecVehicleVBTrans[VecNum - 1].x, m_vecVehicleVBTrans[VecNum - 1].y)
		, D2D1::Point2(m_vecVehicleVBTrans[0].x, m_vecVehicleVBTrans[0].y)
		, gpBrush, 1);

	//操控行为相关
	mpSteering->Render();
	
	//std::wstringstream ss;
	//std::wstring str;
	//ss << L"目标";
	//ss >> str;
	////OutputDebugString(str.c_str());
	//gpRenderTarget->DrawText(str.c_str(), str.size(), gpTextFormat, D2D1::RectF(mpSteering->GetTargetPos().x, mpSteering->GetTargetPos().y
	//	, mpSteering->GetTargetPos().x + 50, mpSteering->GetTargetPos().y + 20), gpBrush);

	//gpBrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 1.0f));
	//ellipse = D2D1::Ellipse(D2D1::Point2F(mPos.x, mPos.y), mdBoundingRadius, mdBoundingRadius);
	//gpRenderTarget->DrawEllipse(ellipse, gpBrush, 1);


}
