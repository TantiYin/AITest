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

//returns a random integer between x and y
inline int   RandInt(int x, int y)
{
	assert(y >= x && "<RandInt>: y is less than x");
	return rand() % (y - x + 1) + x;
}

//returns a random double between zero and 1
inline double RandFloat() { return ((rand()) / (RAND_MAX + 1.0)); }

//returns a random double in the range -1 < n < 1
inline double RandomClamped() { return RandFloat() - RandFloat(); }
