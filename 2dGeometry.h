#pragma once

#include <cmath>

//----------------------------- TwoCirclesOverlapped ---------------------
//
//  Returns true if the two circles overlap
//------------------------------------------------------------------------
inline bool TwoCirclesOverlapped(double x1, double y1, double r1,
	double x2, double y2, double r2)
{
	double DistBetweenCenters = sqrt((x1 - x2) * (x1 - x2) +
		(y1 - y2) * (y1 - y2));

	if ((DistBetweenCenters < (r1 + r2)) || (DistBetweenCenters < fabs(r1 - r2)))
	{
		return true;
	}

	return false;
}
//----------------------------- TwoCirclesOverlapped ---------------------
//
//  Returns true if the two circles overlap
//------------------------------------------------------------------------
inline bool TwoCirclesOverlapped(Vector2 c1, double r1,
	Vector2 c2, double r2)
{
	double DistBetweenCenters = sqrt((c1.x - c2.x) * (c1.x - c2.x) +
		(c1.y - c2.y) * (c1.y - c2.y));

	if ((DistBetweenCenters < (r1 + r2)) || (DistBetweenCenters < fabs(r1 - r2)))
	{
		return true;
	}

	return false;
}
