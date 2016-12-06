#include "vehicle.h"
#include "steeringBehavior.h"
#include "d2d1.h"

extern ID2D1HwndRenderTarget* gpRenderTarget;
extern ID2D1SolidColorBrush* gpBrush;

Vehicle::Vehicle(int id, Vector2 pos, double r, Vector2 v, Vector2 head, double m, double maxspeed, double maxforce):
MovingEntity(id, pos, r, v, head, m, maxspeed, maxforce)
{
	//set up the steering behavior class
	mpSteering = new SteeringBehavior(this);
	mpSteering->SeekOn();

	const int NumVehicleVerts = 3;

	Vector2 vehicle[NumVehicleVerts] = { Vector2(-1.0f,0.6f),
		Vector2(1.0f,0.0f),
		Vector2(-1.0f,-0.6f) };

	//setup the vertex buffers and calculate the bounding radius
	for (int vtx = 0; vtx < NumVehicleVerts; ++vtx)
	{
		mvecVehicleVB.push_back(vehicle[vtx]);
	}
}

void Vehicle::Update(double t)
{

	//keep a record of its old position so we can update its cell later
	//in this method
	Vector2 OldPos = Pos();
	Vector2 TargetPos = Vector2(50, 100);
	mpSteering->SetTargetPos(TargetPos);

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
}

void Vehicle::Render()
{
	D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2F(mpSteering->GetTargetPos().x, mpSteering->GetTargetPos().y), mdBoundingRadius, mdBoundingRadius);
	gpBrush->SetColor(D2D1::ColorF(1.0f, 0.0f, 0.0f));
	gpRenderTarget->DrawEllipse(ellipse, gpBrush, 1);

	gpBrush->SetColor(D2D1::ColorF(0.0f, 0.0f, 1.0f));
	ellipse = D2D1::Ellipse(D2D1::Point2F(mPos.x, mPos.y), mdBoundingRadius, mdBoundingRadius);
	gpRenderTarget->DrawEllipse(ellipse, gpBrush, 1);


}
