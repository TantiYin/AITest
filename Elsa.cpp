#include "headers.h"
#include <iostream>
using namespace std;

Elsa::Elsa(int id, Vector2 pos, double r) :BaseGameEntity(id, pos, r),mUrineVolume(0)
{
	mFSM = new StateMachine<Elsa>(this);
	mFSM->SetCurrentState(CleanHouse::Instance());
	mFSM->SetGlobalState(WifeGlobalState::Instance());
}

StateMachine<Elsa>* Elsa::GetFSM()
{
	return mFSM;
}

void Elsa::Update()
{
	if (mFSM)
	{
		mFSM->Update();
	}
}
bool Elsa::HandleMessage(const Telegram& msg)
{
	return mFSM->HandleMessage(msg);
}

void Elsa::CleanFloor()
{
	mUrineVolume += 20;
	cout << "\nElsa: clean floor";
}

void Elsa::Pee()
{
	mUrineVolume = 0 ;
	cout << "\nElsa: pee";
}

bool Elsa::IsEmpty()
{
	if (mUrineVolume <= 0)
	{
		return true;
	}
	return false;
}

bool Elsa::IsFull()
{
	if (mUrineVolume >= 100)
	{
		return true;
	}
	return false;
}