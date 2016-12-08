#pragma once


#include "baseEntity.h"
#include "stateMachine.h"

class Elsa: public BaseGameEntity
{
public:
	Elsa(int id, Vector2 pos, double r);
	~Elsa(){}

	StateMachine<Elsa>* GetFSM();
	void Update(double t);
	bool HandleMessage(const Telegram&);

	void CleanFloor();
	void Pee();

	bool IsEmpty();
	bool IsFull();
private:
	StateMachine<Elsa> * mFSM;

	int mUrineVolume;
};



