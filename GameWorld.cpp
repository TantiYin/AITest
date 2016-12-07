#include "GameWorld.h"
#include "vector2d.h"
#include "vehicle.h"


GameWorld::GameWorld()
{
	mVehicle = new Vehicle(this, Vector2(0, 0), 2, Vector2(0, 10), Vector2(0, 1), 10, 20, 30);
}

GameWorld::~GameWorld()
{
	delete mVehicle;
}

void GameWorld::Update(double t)
{
	mVehicle->Update(t);

}

void GameWorld::Render()
{
	mVehicle->Render();
}
