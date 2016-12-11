#pragma once

#include <cmath>
#include "utils.h"

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


//--------------------LineIntersection2D-------------------------
//
//	Given 2 lines in 2D space AB, CD this returns true if an 
//	intersection occurs.
//
//----------------------------------------------------------------- 

inline bool LineIntersection2D(Vector2 A,
    Vector2 B,
    Vector2 C,
    Vector2 D)
{
    double rTop = (A.y - C.y)*(D.x - C.x) - (A.x - C.x)*(D.y - C.y);
    double sTop = (A.y - C.y)*(B.x - A.x) - (A.x - C.x)*(B.y - A.y);

    double Bot = (B.x - A.x)*(D.y - C.y) - (B.y - A.y)*(D.x - C.x);

    if (Bot == 0)//parallel
    {
        return false;
    }

    double invBot = 1.0 / Bot;
    double r = rTop * invBot;
    double s = sTop * invBot;

    if ((r > 0) && (r < 1) && (s > 0) && (s < 1))
    {
        //lines intersect
        return true;
    }

    //lines do not intersect
    return false;
}

//--------------------LineIntersection2D-------------------------
//
//	Given 2 lines in 2D space AB, CD this returns true if an 
//	intersection occurs and sets dist to the distance the intersection
//  occurs along AB
//
//----------------------------------------------------------------- 

inline bool LineIntersection2D(Vector2 A,
    Vector2 B,
    Vector2 C,
    Vector2 D,
    double &dist)
{

    double rTop = (A.y - C.y)*(D.x - C.x) - (A.x - C.x)*(D.y - C.y);
    double sTop = (A.y - C.y)*(B.x - A.x) - (A.x - C.x)*(B.y - A.y);

    double Bot = (B.x - A.x)*(D.y - C.y) - (B.y - A.y)*(D.x - C.x);


    if (Bot == 0)//parallel
    {
        if (isEqual(rTop, 0) && isEqual(sTop, 0))
        {
            return true;
        }
        return false;
    }

    double r = rTop / Bot;
    double s = sTop / Bot;

    if ((r > 0) && (r < 1) && (s > 0) && (s < 1))
    {
        dist = Vec2DDistance(A, B) * r;

        return true;
    }

    else
    {
        dist = 0;

        return false;
    }
}

//-------------------- LineIntersection2D-------------------------
//
//	Given 2 lines in 2D space AB, CD this returns true if an 
//	intersection occurs and sets dist to the distance the intersection
//  occurs along AB. Also sets the 2d vector point to the point of
//  intersection
//----------------------------------------------------------------- 
inline bool LineIntersection2D(Vector2   A,
    Vector2   B,
    Vector2   C,
    Vector2   D,
    double&     dist,
    Vector2&  point)
{

    double rTop = (A.y - C.y)*(D.x - C.x) - (A.x - C.x)*(D.y - C.y);
    double rBot = (B.x - A.x)*(D.y - C.y) - (B.y - A.y)*(D.x - C.x);

    double sTop = (A.y - C.y)*(B.x - A.x) - (A.x - C.x)*(B.y - A.y);
    double sBot = (B.x - A.x)*(D.y - C.y) - (B.y - A.y)*(D.x - C.x);

    if ((rBot == 0) || (sBot == 0))
    {
        //lines are parallel
        return false;
    }

    double r = rTop / rBot;
    double s = sTop / sBot;

    if ((r > 0) && (r < 1) && (s > 0) && (s < 1))
    {
        dist = Vec2DDistance(A, B) * r;

        point = A + r * (B - A);

        return true;
    }

    else
    {
        dist = 0;

        return false;
    }
}