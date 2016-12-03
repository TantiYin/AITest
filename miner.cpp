#include "miner.h"
#include "stateOfMiner.h"

Miner::Miner(int id, Vector2 pos, double r) :BaseGameEntity(id, pos, r)
{
	mEnergy = 100;
	mWater = 100;
	mGoldAmount = 0;
	mLocation = shack;
	mFSM = new StateMachine<Miner>(this);
	mFSM->SetCurrentState(GoHomeAndSleepTilRested::Instance());
}

Miner::~Miner()
{
}

StateMachine<Miner>* Miner::GetFSM()
{
	return mFSM;
}

void Miner::Update(double t)
{
	if (mFSM)
	{
		mFSM->Update(t);
	}
}

bool Miner::HandleMessage(const Telegram& msg)
{
	return mFSM->HandleMessage(msg);
}

void Miner::AddGold(int n)
{
	mGoldAmount += n;
}

void Miner::DepositGold(int n)
{
	mGoldAmount = 0;
}

bool Miner::IsPocketFull()
{
	if (mGoldAmount >= 100)
	{
		return true;
	}
	return false;
}

bool Miner::IsThirsty()
{
	if (mWater <= 0)
	{
		return true;
	}
	return false;
}

void Miner::DrinkWater(int n)
{
	mWater = 100;
}

void Miner::LoseWater(int n)
{
	mWater -= n;
}

bool Miner::IsFullWater()
{
	if (mWater >= 100)
	{
		return true;
	}
	return false;
}

bool Miner::IsTired()
{
	if (mEnergy <= 0)
	{
		return true;
	}
	return false;
}

void Miner::Recover()
{
	mEnergy += 20;
}

void Miner::LoseEnergy(int n)
{
	mEnergy -= n;
}

bool Miner::IsFullEnergy()
{
	if (mEnergy >= 100)
	{
		return true;
	}
	return false;
}

int Miner::GetEnergy()
{
	return mEnergy;
}

int Miner::GetWater()
{
	return mWater;
}

int Miner::GetGold()
{
	return mGoldAmount;
}