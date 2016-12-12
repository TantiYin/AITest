#pragma once

#include <list>
#include "vector2d.h"
#include <cassert>

class Path
{
private:
	std::list<Vector2> mWayPoints;
	//points to the current waypoint
	std::list<Vector2>::iterator  curWaypoint;

	//flag to indicate if the path should be looped
	//(The last waypoint connected to the first)
	bool                           m_bLooped;

public:

	Path() :m_bLooped(false) {}

	//constructor for creating a path with initial random waypoints. MinX/Y
	//& MaxX/Y define the bounding box of the path.
	Path(int    NumWaypoints,
		double MinX,
		double MinY,
		double MaxX,
		double MaxY,
		bool   looped) :m_bLooped(looped)
	{
		CreateRandomPath(NumWaypoints, MinX, MinY, MaxX, MaxY);

		curWaypoint = mWayPoints.begin();
	}


	//returns the current waypoint
	Vector2    CurrentWaypoint()const { return *curWaypoint; }

	//returns true if the end of the list has been reached
	bool        Finished() { return !(curWaypoint != mWayPoints.end()); }

	//moves the iterator on to the next waypoint in the list
	inline void SetNextWaypoint();

	//creates a random path which is bound by rectangle described by
	//the min/max values
	std::list<Vector2> CreateRandomPath(int    NumWaypoints,
		double MinX,
		double MinY,
		double MaxX,
		double MaxY);


	void LoopOn() { m_bLooped = true; }
	void LoopOff() { m_bLooped = false; }
	//methods for setting the path with either another Path or a list of vectors
	void Set(std::list<Vector2> new_path) { mWayPoints = new_path; curWaypoint = mWayPoints.begin(); }
	void Set(const Path& path) { mWayPoints = path.GetPath(); curWaypoint = mWayPoints.begin(); }


	void Clear() { mWayPoints.clear(); }

	std::list<Vector2> GetPath()const { return mWayPoints; }

	//renders the path in orange
	void Render()const;
};

//-------------------- Methods -------------------------------------------

inline void Path::SetNextWaypoint()
{
	assert(mWayPoints.size() > 0);

	if (++curWaypoint == mWayPoints.end())
	{
		if (m_bLooped)
		{
			curWaypoint = mWayPoints.begin();
		}
	}
}
