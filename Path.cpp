#include "Path.h"
#include <d2d1.h>
#include "utils.h"
#include "transformation.h"

extern ID2D1HwndRenderTarget* gpRenderTarget;
extern ID2D1SolidColorBrush* gpBrush;


std::list<Vector2> Path::CreateRandomPath(int NumWaypoints, double MinX, double MinY, double MaxX, double MaxY)
{
	mWayPoints.clear();

	double midX = (MaxX + MinX) / 2.0;
	double midY = (MaxY + MinY) / 2.0;

	double smaller = min(midX, midY);

	double spacing = TwoPi / (double)NumWaypoints;

	for (int i = 0; i < NumWaypoints; ++i)
	{
		double RadialDist = RandInRange(smaller*0.2f, smaller);

		Vector2 temp(RadialDist, 0.0f);

		Vec2DRotateAroundOrigin(temp, i*spacing);

		temp.x += midX; temp.y += midY;

		mWayPoints.push_back(temp);

	}

	curWaypoint = mWayPoints.begin();

	return mWayPoints;

}

void Path::Render() const
{
	gpBrush->SetColor(D2D1::ColorF(0.0f, 1.0f, 0.0f));
	for (auto it = mWayPoints.begin(); it != mWayPoints.end();)
	{
		D2D1_POINT_2F from = D2D1::Point2(it->x, it->y);
		++it;
		D2D1_POINT_2F to = D2D1::Point2(it->x, it->y);
		gpRenderTarget->DrawLine(from, to, gpBrush, 1);
	}
}
