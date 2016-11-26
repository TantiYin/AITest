#pragma once


#include "state.h"
#include "stdio.h"

template<class entity_type>
class StateMachine
{
public:
	StateMachine(entity_type* owner) :mpOwner(owner),
		mpCurrentState(NULL),
		mpPreviousState(NULL),
		mpGlobalState(NULL)
	{}
	//初始化
	void SetCurrentState(State<entity_type>* s){ mpCurrentState = s; }
	void SetPreviousState(State<entity_type>* s){ mpPreviousState = s; }
	void SetGlobalState(State<entity_type>* s){ mpGlobalState = s; }
	//更新FSM
	void Update() const
	{
		if (mpGlobalState)
		{
			mpGlobalState->Execute(mpOwner);
		}
		if (mpCurrentState)
		{
			mpCurrentState->Execute(mpOwner);
		}

	}

	bool HandleMessage(const Telegram& msg)
	{
		if (mpCurrentState && mpCurrentState->OnMessage(mpOwner, msg))
		{
			return true;
		}
		if (mpGlobalState && mpGlobalState->OnMessage(mpOwner, msg))
		{
			return true;
		}
		return false;
	}

	void ChangeState(State<entity_type>* s)
	{
		mpPreviousState = mpCurrentState;
		mpCurrentState->Exit(mpOwner);
		mpCurrentState = s;
		mpCurrentState->Enter(mpOwner);
	}
	

	void RevertToPreviousState()
	{
		ChangeState(mpPreviousState);
	}
	//访问状态
	State<entity_type>* CurrentState(){ return mpCurrentState; }
	State<entity_type>* GlobalState(){ return mpGlobalState; }
	State<entity_type>* PreviousState(){ return mpPreviousState; }

	bool IsInState(const State<entity_type>& state)
	{
		return true;
	}

private:

	entity_type * mpOwner;
	State<entity_type> *mpCurrentState;
	State<entity_type> * mpPreviousState;
	State<entity_type> * mpGlobalState;

};



