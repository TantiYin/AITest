#pragma once

#include "baseEntity.h"
#include "2dGeometry.h"

//------------------------- Overlapped -----------------------------------
//
//  tests to see if an entity is overlapping any of a number of entities
//  stored in a std container
//------------------------------------------------------------------------
template <class T, class conT>
bool Overlapped(const T* ob, const conT& conOb, double MinDistBetweenObstacles = 40.0)
{
	typename conT::const_iterator it;

	for (it = conOb.begin(); it != conOb.end(); ++it)
	{
		if (TwoCirclesOverlapped(ob->Pos(),
			ob->BRadius() + MinDistBetweenObstacles,
			(*it)->Pos(),
			(*it)->BRadius()))
		{
			return true;
		}
	}

	return false;
}

//----------------------- TagNeighbors ----------------------------------
//
//  tags any entities contained in a std container that are within the
//  radius of the single entity parameter
//------------------------------------------------------------------------
template <class T, class conT>
void TagNeighbors(const T& entity, conT& ContainerOfEntities, double radius)
{
	//iterate through all entities checking for range
	for (typename conT::iterator curEntity = ContainerOfEntities.begin();
	curEntity != ContainerOfEntities.end();
		++curEntity)
	{
		//first clear any current tag
		(*curEntity)->UnTag();

		Vector2 to = (*curEntity)->Pos() - entity->Pos();

		//the bounding radius of the other is taken into account by adding it 
		//to the range
		double range = radius + (*curEntity)->BRadius();

		//if entity within range, tag for further consideration. (working in
		//distance-squared space to avoid sqrts)
		if (((*curEntity) != entity) && (to.LengthSq() < range*range))
		{
			(*curEntity)->Tag();
		}

	}//next entity
}