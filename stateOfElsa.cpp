#include "stateOfElsa.h"
#include "Elsa.h"
#include "telegram.h"
#include "entityName.h"
#include "MessageDispatcher.h"
#include "messageType.h"
#include <iostream>
using namespace std;

void CleanHouse::Enter(Elsa* target)
{

}

void CleanHouse::Execute(Elsa* target)
{
	target->CleanFloor();
}

void CleanHouse::Exit(Elsa* target)
{

}

bool CleanHouse::OnMessage(Elsa*, const Telegram&)
{
	return false;
}

void GotoBathroom::Enter(Elsa* target)
{

}

void GotoBathroom::Execute(Elsa* target)
{
	if (target->IsEmpty())
	{
		target->GetFSM()->RevertToPreviousState();
	}
	else
	{
		target->Pee();
	}
	
}

void GotoBathroom::Exit(Elsa* target)
{

}

bool GotoBathroom::OnMessage(Elsa*, const Telegram&)
{
	return false;
}

void CookStew::Enter(Elsa* target)
{
	MessageDispatcher::Instance()->DispatchMessage(3, target->GetID(), target->GetID(), Msg_StewReady, NULL);
}

void CookStew::Execute(Elsa* target)
{
	wcout << L"\n" << GetNameOfEntity(target->GetID()) << L": cooking...";
}

void CookStew::Exit(Elsa*)
{

}

bool CookStew::OnMessage(Elsa* target, const Telegram& telegram)
{
	switch (telegram.Msg)
	{
	case Msg_StewReady:
	{
						  wcout << L"\n" << GetNameOfEntity(target->GetID()) << L": stew ready";
						  MessageDispatcher::Instance()->DispatchMessage(0, target->GetID(), ent_Miner_Bob, Msg_StewReady, NULL);
						  target->GetFSM()->ChangeState(CleanHouse::Instance());

	}
		return true;
	default:
		break;
	}
	return false;
}

void WifeGlobalState::Enter(Elsa* target)
{
	
}

void WifeGlobalState::Execute(Elsa* target)
{
	if (target->IsFull())
	{
		target->GetFSM()->ChangeState(new GotoBathroom());
	}
}

void WifeGlobalState::Exit(Elsa* target)
{

}

bool WifeGlobalState::OnMessage(Elsa* target, const Telegram& telegram)
{
	switch (telegram.Msg)
	{
	case Msg_HiHoneyImHome:
	{
							  wcout << L"\n" << GetNameOfEntity(target->GetID()) << L": I will cook stew";
							  target->GetFSM()->ChangeState(CookStew::Instance());
							
	} 
		return true;
	default:
		break;
	}
	return false;
}
