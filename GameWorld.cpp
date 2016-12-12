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
	mVehicle = new Vehicle(this, Vector2(m_cxClient / 2, m_cyClient / 2), 6, Vector2(0, 10), Vector2(0, 1), 2, 50, 100);

	CreateObstacles();
    CreateWalls();

	double border = 30;
	m_pPath = new Path(5, border, border, m_cxClient - border, m_cyClient - border, true);
	mVehicle->Steering()->SetPath(m_pPath->GetPath());

}

GameWorld::~GameWorld()
{
	delete mVehicle;
	delete m_pPath;
}

void GameWorld::Update(double t)
{
	mVehicle->Update(t);
}

void GameWorld::Render()
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

	//ÓÎÏ·¶ÔÏó
	mVehicle->Render();

	for (const auto& ob : m_Obstacles)
	{
		ob->Render();
	}
    for (const auto& wall: m_Walls)
    {
        wall.Render();
    }

	m_pPath->Render();
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
