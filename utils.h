#pragma once

#include <cstdlib>

//returns a random double between zero and 1
inline double RandFloat() { return ((rand()) / (RAND_MAX + 1.0)); }

//returns a random double in the range -1 < n < 1
inline double RandomClamped() { return RandFloat() - RandFloat(); }
