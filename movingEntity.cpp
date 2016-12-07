#include "movingEntity.h"
#include <cassert>



void MovingEntity::SetHeading(Vector2 new_heading)
{
	assert((new_heading.LengthSq() - 1.0) < 0.00001);

	mHeading = new_heading;

	//the side vector must always be perpendicular to the heading
	mSide = mHeading.Perp();
}
