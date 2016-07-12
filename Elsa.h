#ifndef _ELSA_H_
#define _ELSA_H_

#include "npc.h"
#include "stateMachine.h"

class Elsa:public BaseGameEntity
{
public:
	Elsa(int id, Vector2 pos, double r);
	~Elsa(){}

	StateMachine<Elsa>* GetFSM();
	void Update();
	bool HandleMessage(const Telegram&);

	void CleanFloor();
	void Pee();

	bool IsEmpty();
	bool IsFull();
private:
	StateMachine<Elsa> * mFSM;

	int mUrineVolume;
};


#endif