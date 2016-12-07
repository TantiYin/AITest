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

	int   cxClient()const { return m_cxClient; }
	int   cyClient()const { return m_cyClient; }

	void SetcxClient(int x) { m_cxClient = x; }
	void SetcyClient(int y) { m_cyClient = y; }

private:
	Vehicle* mVehicle;

	//the position of the crosshair
	Vector2                      m_vCrosshair;


	//local copy of client window dimensions
	int                           m_cxClient, m_cyClient;
};
