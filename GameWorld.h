#pragma once

#include <windows.h>
#include "vector2d.h"
#include "EntityFunction.h"
#include <vector>
#include "wall2d.h"
#include "Path.h"

class Vehicle;

class GameWorld
{
private:
	//local copy of client window dimensions
	int                           m_cxClient, m_cyClient;

	//the position of the crosshair
	Vector2                      m_vCrosshair;

	Vehicle* mVehicle;
	Vehicle* mVehicle2;

	//any obstacles
	std::vector<BaseGameEntity*>  m_Obstacles;

    //container containing any walls in the environment
    std::vector<Wall2d>           m_Walls;

	//any path we may create for the vehicles to follow
	Path*                         m_pPath = nullptr;

private:

	void CreateObstacles();
    void CreateWalls();

public:
	GameWorld(int x, int y);
	virtual ~GameWorld();

	void Update(double t);
	void Render();

	int   cxClient()const { return m_cxClient; }
	int   cyClient()const { return m_cyClient; }
	
	void SetcxClient(int x) { m_cxClient = x; }
	void SetcyClient(int y) { m_cyClient = y; }

	Vector2    Crosshair()const { return m_vCrosshair; }
	void        SetCrosshair(POINTS p);

	void  TagObstaclesWithinViewRange(BaseGameEntity* pVehicle, double range)
	{
		TagNeighbors(pVehicle, m_Obstacles, range);
	}

	const std::vector<BaseGameEntity*>& Obstacles()const { return m_Obstacles; }

    const std::vector<Wall2d>&          Walls(){ return m_Walls; }

};
