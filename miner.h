#ifndef _MINER_H_
#define _MINER_H_

#include "npc.h"
#include "stateMachine.h"

class Miner :public BaseGameEntity
{
public:
	Miner(int id, Vector2 pos, double r);
	~Miner();

	StateMachine<Miner>* GetFSM();
	void Update();
	bool HandleMessage(const Telegram&);

	//bool IsInGoldmine();
	//void GotoGoldmine();
	void AddGold(int);
	void DepositGold(int);
	bool IsPocketFull();

	bool IsThirsty();
	void DrinkWater(int);
	void LoseWater(int);
	bool IsFullWater();

	bool IsTired();
	void Recover();
	void LoseEnergy(int);
	bool IsFullEnergy();


	int GetEnergy();
	int GetWater();
	int GetGold();


	location_type GetLocation(){ return mLocation; }
	void          ChangeLocation(location_type loc){ mLocation = loc; }
private:
	int mEnergy;
	int mWater;
	//int mBagSize;
	int mGoldAmount;
	location_type         mLocation;
	StateMachine<Miner> * mFSM;

};


#endif