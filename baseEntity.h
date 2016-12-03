#pragma once


#include "vector2d.h"

struct Telegram;

class BaseGameEntity
{
public:
	BaseGameEntity(int id, Vector2 pos, double r):mId(id), mPos(pos), m_dBoundingRadius(r){}

	virtual ~BaseGameEntity(){}
	virtual void Update(double t) = 0;
	virtual bool HandleMessage(const Telegram&) = 0;

	int GetID(){ return mId; }
	void SetID(int id){ mId = id; }

	Vector2    Pos()const{ return mPos; }
	void       SetPos(Vector2 new_pos){ mPos = new_pos; }

	double     BRadius()const{ return m_dBoundingRadius; }
	void       SetBRadius(double r){ m_dBoundingRadius = r; }

protected:
	int mId;//every entity has a unique id
	Vector2 mPos;//

	//the length of this object's bounding radius
	double    m_dBoundingRadius;

};



