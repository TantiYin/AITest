#pragma once


#include "vector2d.h"

struct Telegram;

class BaseGameEntity
{
public:
	BaseGameEntity(int id, Vector2 pos, double r): mId(id), mPos(pos), mdBoundingRadius(r){}

	virtual ~BaseGameEntity(){}
	virtual void Update(double t) = 0;
	virtual bool HandleMessage(const Telegram&) = 0;
	virtual void Render() = 0;

	int GetID(){ return mId; }
	void SetID(int id){ mId = id; }

	Vector2    Pos()const{ return mPos; }
	void       SetPos(Vector2 newPos){ mPos = newPos; }

	double     BRadius()const{ return mdBoundingRadius; }
	void       SetBRadius(double r){ mdBoundingRadius = r; }

protected:
	int mId;//every entity has a unique id
	Vector2 mPos;//

	//the length of this object's bounding radius
	double    mdBoundingRadius;

};



