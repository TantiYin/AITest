#pragma once


#include "vector2d.h"

struct Telegram;

class BaseGameEntity
{
public:
	BaseGameEntity(int entityType, Vector2 pos, double r): mId(NextValidID()), mEntityType(entityType), mPos(pos), mdBoundingRadius(r){}

	virtual ~BaseGameEntity(){}
	virtual void Update(double t) = 0;
	virtual bool HandleMessage(const Telegram&) = 0;
	virtual void Render() = 0;

	int GetID(){ return mId; }
	void SetID(int id){ mId = id; }

	int GetEntityType() { return mEntityType; }
	void SetEntityType(int entityType) { mEntityType = entityType; }

	Vector2    Pos()const{ return mPos; }
	void       SetPos(Vector2 newPos){ mPos = newPos; }

	double     BRadius()const{ return mdBoundingRadius; }
	void       SetBRadius(double r){ mdBoundingRadius = r; }

private:
	//used by the constructor to give each entity a unique ID
	int NextValidID() { static int NextID = 0; return NextID++; }

protected:
	int mId;//every entity has a unique id
	int mEntityType;
	Vector2 mPos;//

	//the length of this object's bounding radius
	double    mdBoundingRadius;

};



