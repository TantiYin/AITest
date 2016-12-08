#pragma once

#include "baseEntity.h"

class Obstacle : public BaseGameEntity
{
public:
	Obstacle(double x,
		double y,
		double r): BaseGameEntity(0, Vector2(x, y), r)
	{}

	Obstacle(Vector2 pos, double radius): BaseGameEntity(0, pos, radius)
	{}

	virtual ~Obstacle() {}
	//this is defined as a pure virtual function in BasegameEntity so
	//it must be implemented
	void      Update(double time_elapsed) {}
	bool HandleMessage(const Telegram&) { return true; }
	void      Render();
};