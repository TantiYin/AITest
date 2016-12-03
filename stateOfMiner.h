#pragma once


#include "state.h"

class Miner;
struct Telegram;

class EnterMineAndDigForGold :public State<Miner>
{
public:
	void Enter(Miner*);
	void Execute(Miner*);
	void Exit(Miner*);

	bool OnMessage(Miner*, const Telegram&);
	static State<Miner>* Instance()
	{
		static State<Miner>* inst = new EnterMineAndDigForGold();
		return inst;
	}
private:

};

class VisitBankAndDepositGold :public State<Miner>
{
public:
	void Enter(Miner*);
	void Execute(Miner*);
	void Exit(Miner*);
	bool OnMessage(Miner*, const Telegram&);
	static State<Miner>* Instance()
	{
		static State<Miner>* inst = new VisitBankAndDepositGold();
		return inst;
	}
private:

};

class GoHomeAndSleepTilRested :public State<Miner>
{
public:
	void Enter(Miner*);
	void Execute(Miner*);
	void Exit(Miner*);
	bool OnMessage(Miner*, const Telegram&);
	static State<Miner>* Instance()
	{
		static State<Miner>* inst = new GoHomeAndSleepTilRested();
		return inst;
	}
private:

};

class QuenchThirst :public State<Miner>
{
public:
	void Enter(Miner*);
	void Execute(Miner*);
	void Exit(Miner*);
	bool OnMessage(Miner*, const Telegram&);
	static State<Miner>* Instance()
	{
		static State<Miner>* inst = new QuenchThirst();
		return inst;
	}
private:

};

class EatStew:public State<Miner>
{
public:
	void Enter(Miner*);
	void Execute(Miner*);
	void Exit(Miner*);
	bool OnMessage(Miner*, const Telegram&);
	static State<Miner>* Instance()
	{
		static State<Miner>* inst = new EatStew();
		return inst;
	}

private:

};


