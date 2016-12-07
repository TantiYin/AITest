#pragma once

#include <windows.h>
#include "vector2d.h"

class Vehicle;

class GameWorld
{
public:
	GameWorld(int x, int y);
	~GameWorld();

	void Update(double t);
	void Render();
	
	Vector2    Crosshair()const { return m_vCrosshair; }
	void        SetCrosshair(POINTS p);
private:
	Vehicle* mVehicle;

	//the position of the crosshair
	Vector2                      m_vCrosshair;
};
