#include "GameWorld.h"
#include "vehicle.h"
#include <d2d1.h>
#include "utils.h"
#include "Obstacle.h"
#include "steeringBehavior.h"

extern ID2D1HwndRenderTarget* gpRenderTarget;
extern ID2D1SolidColorBrush* gpBrush;
extern IDWriteTextFormat* gpTextFormat;

GameWorld::GameWorld(int x, int y): m_cxClient(x), m_cyClient(y), m_vCrosshair(Vector2(x / 2.0, y / 2.0))
{
	if (bShowWall)
	{
		CreateWalls();
	}
	if (bShowObstacle)
	{
		CreateObstacles();
	}
	if (bShowPath)
	{
		double border = 30;
		m_pPath = new Path(5, border, border, m_cxClient - border, m_cyClient - border, true);
	}

	Vehicle* TmpVehicle = new Vehicle(this, Vector2(RandInRange(0, x), RandInRange(0, y)), 6, Vector2(0, 10), Vector2(0, 1), 2, 50, 100);
	TmpVehicle->Steering()->WanderOn();
	TmpVehicle->Steering()->FlockingOn();

	mVehicles.push_back(TmpVehicle);

	TmpVehicle = new Vehicle(this, Vector2(RandInRange(0, x), RandInRange(0, y)), 6, Vector2(0, 10), Vector2(0, 1), 2, 50, 100);
	TmpVehicle->Steering()->WanderOn();
	TmpVehicle->Steering()->FlockingOn();

	mVehicles.push_back(TmpVehicle);

	TmpVehicle = new Vehicle(this, Vector2(RandInRange(0, x), RandInRange(0, y)), 6, Vector2(0, 10), Vector2(0, 1), 2, 50, 100);
	TmpVehicle->Steering()->WanderOn();
	TmpVehicle->Steering()->FlockingOn();

	mVehicles.push_back(TmpVehicle);
}

GameWorld::~GameWorld()
{
	for (auto i = 0; i < m_Obstacles.size(); ++i)
	{
		delete m_Obstacles[i];
	}

	for (auto Tmp : mVehicles)
	{
		delete Tmp;
	}

	if (m_pPath != nullptr)
	{
		delete m_pPath;
	}
}

void GameWorld::Update(double t)
{
	for (auto i = 0; i < mVehicles.size(); ++i)
	{
		mVehicles[i]->Update(t);
	}
}

void GameWorld::Render()
{
	//是否绘制准星
	if (bShowCrosshair)
	{
		//and finally the crosshair
		gpBrush->SetColor(D2D1::ColorF(1.0, 0.0, 0.0));
		D2D1_ELLIPSE ellipse = D2D1::Ellipse(D2D1::Point2(m_vCrosshair.x, m_vCrosshair.y), 4, 4);
		gpRenderTarget->DrawEllipse(ellipse, gpBrush, 1);
		gpRenderTarget->DrawLine(D2D1::Point2(m_vCrosshair.x - 8, m_vCrosshair.y), D2D1::Point2(m_vCrosshair.x + 8, m_vCrosshair.y), gpBrush, 1);
		gpRenderTarget->DrawLine(D2D1::Point2(m_vCrosshair.x, m_vCrosshair.y - 8), D2D1::Point2(m_vCrosshair.x, m_vCrosshair.y + 8), gpBrush, 1);
		//D2D1_SIZE_F size = gpRenderTarget->GetSize();
		WCHAR LineText[30] = L"Click to move crosshair";
		gpRenderTarget->DrawText(LineText, ARRAYSIZE(LineText) - 1, gpTextFormat, D2D1::RectF(5, cyClient() - 20, cxClient(), cyClient()), gpBrush);
	}

	//游戏对象
	for (auto i = 0; i < mVehicles.size(); ++i)
	{
		mVehicles[i]->Render();
	}

	//是否绘制墙
	if (bShowWall)
	{
		for (const auto& wall: m_Walls)
		{
			wall.Render();
		}
	}

	//是否绘制障碍物
	if (bShowObstacle)
	{
		for (const auto& ob : m_Obstacles)
		{
			ob->Render();
		}
	}

	//是否绘制路线
	if (bShowPath)
	{
		m_pPath->Render();
	}
}

void GameWorld::SetCrosshair(POINTS p)
{
	m_vCrosshair.x = (double)p.x;
	m_vCrosshair.y = (double)p.y;
}

void GameWorld::CreateObstacles()
{
	const int NumObstacles = 10;
	const int MinObstacleRadius = 20;
	const int MaxObstacleRadius = 50;

	//create a number of randomly sized tiddlywinks
	for (int o = 0; o < NumObstacles; ++o)
	{
		bool bOverlapped = true;

		//keep creating tiddlywinks until we find one that doesn't overlap
		//any others.Sometimes this can get into an endless loop because the
		//obstacle has nowhere to fit. We test for this case and exit accordingly

		int NumTrys = 0; int NumAllowableTrys = 2000;

		while (bOverlapped)
		{
			NumTrys++;

			if (NumTrys > NumAllowableTrys) return;

			int radius = RandInt((int)MinObstacleRadius, (int)MaxObstacleRadius);

			const int border = 10;
			const int MinGapBetweenObstacles = 20;

			Obstacle* ob = new Obstacle(RandInt(radius + border, m_cxClient - radius - border),
				RandInt(radius + border, m_cyClient - radius - 30 - border),
				radius);

			if (!Overlapped(ob, m_Obstacles, MinGapBetweenObstacles))
			{
				//its not overlapped so we can add it
				m_Obstacles.push_back(ob);

				bOverlapped = false;
			}

			else
			{
				delete ob;
			}
		}
	}
}

void GameWorld::CreateWalls()
{
    //create the walls  
    double bordersize = 20.0;
    double CornerSize = 0.2;
    double vDist = m_cyClient - 2 * bordersize;
    double hDist = m_cxClient - 2 * bordersize;

    const int NumWallVerts = 8;

    Vector2 walls[NumWallVerts] = { Vector2(hDist*CornerSize + bordersize, bordersize),
        Vector2(m_cxClient - bordersize - hDist*CornerSize, bordersize),
        Vector2(m_cxClient - bordersize, bordersize + vDist*CornerSize),
        Vector2(m_cxClient - bordersize, m_cyClient - bordersize - vDist*CornerSize),

        Vector2(m_cxClient - bordersize - hDist*CornerSize, m_cyClient - bordersize),
        Vector2(hDist*CornerSize + bordersize, m_cyClient - bordersize),
        Vector2(bordersize, m_cyClient - bordersize - vDist*CornerSize),
        Vector2(bordersize, bordersize + vDist*CornerSize) };

    for (int w = 0; w<NumWallVerts - 1; ++w)
    {
        m_Walls.push_back(Wall2d(walls[w], walls[w + 1]));
    }

    m_Walls.push_back(Wall2d(walls[NumWallVerts - 1], walls[0]));

}
