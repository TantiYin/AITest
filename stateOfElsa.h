#pragma once


#include "state.h"
class Elsa;
struct Telegram;

class CleanHouse:public State<Elsa>
{
public:
	void Enter(Elsa*);
	void Execute(Elsa*);
	void Exit(Elsa*);
	bool OnMessage(Elsa*, const Telegram&);
	static State<Elsa>* Instance()
	{
		static State<Elsa>* inst = new CleanHouse();
		return inst;
	}
private:

};

class GotoBathroom:public State<Elsa>
{
public:
	void Enter(Elsa*);
	void Execute(Elsa*);
	void Exit(Elsa*);
	bool OnMessage(Elsa*, const Telegram&);
	static State<Elsa>* Instance()
	{
		static State<Elsa>* inst = new GotoBathroom();
		return inst;
	}
private:

};

class CookStew:public State<Elsa>
{
public:
	void Enter(Elsa*);
	void Execute(Elsa*);
	void Exit(Elsa*);
	bool OnMessage(Elsa*, const Telegram&);
	static State<Elsa>* Instance()
	{
		static State<Elsa>* inst = new CookStew();
		return inst;
	}
private:

};

class WifeGlobalState:public State<Elsa>
{
public:
	void Enter(Elsa*);
	void Execute(Elsa*);
	void Exit(Elsa*);
	bool OnMessage(Elsa*, const Telegram&);
	static State<Elsa>* Instance()
	{
		static State<Elsa>* inst = new WifeGlobalState();
		return inst;
	}
private:

};





