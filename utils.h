#pragma once

#include <limits>
#include <cstdlib>
#include <cassert>

//a few useful constants
const int     MaxInt = (std::numeric_limits<int>::max)();
const double  MaxDouble = (std::numeric_limits<double>::max)();
const double  MinDouble = (std::numeric_limits<double>::min)();
const float   MaxFloat = (std::numeric_limits<float>::max)();
const float   MinFloat = (std::numeric_limits<float>::min)();

const double   Pi = 3.14159;
const double   TwoPi = Pi * 2;
const double   HalfPi = Pi / 2;
const double   QuarterPi = Pi / 4;

//----------------------------------------------------------------------------
//  some random number functions.
//----------------------------------------------------------------------------

//returns a random integer between x and y
inline int   RandInt(int x, int y)
{
	assert(y >= x && "<RandInt>: y is less than x");
	return rand() % (y - x + 1) + x;
}

//returns a random double between zero and 1
inline double RandFloat() { return ((rand()) / (RAND_MAX + 1.0)); }

inline double RandInRange(double x, double y)
{
	return x + RandFloat()*(y - x);
}

//returns a random double in the range -1 < n < 1
inline double RandomClamped() { return RandFloat() - RandFloat(); }


//clamps the first argument between the second two
template <class T, class U, class V>
inline void Clamp(T& arg, const U& minVal, const V& maxVal)
{
	assert(((double)minVal < (double)maxVal) && "<Clamp>MaxVal < MinVal!");

	if (arg < (T)minVal)
	{
		arg = (T)minVal;
	}

	if (arg > (T)maxVal)
	{
		arg = (T)maxVal;
	}
}

//compares two real numbers. Returns true if they are equal
inline bool isEqual(float a, float b)
{
    if (fabs(a - b) < 1E-12)
    {
        return true;
    }

    return false;
}

inline bool isEqual(double a, double b)
{
    if (fabs(a - b) < 1E-12)
    {
        return true;
    }

    return false;
}