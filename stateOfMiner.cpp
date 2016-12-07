#include "stateOfMiner.h"
#include "miner.h"
#include "telegram.h"
#include "entityName.h"
#include "MessageDispatcher.h"
#include "messageType.h"
#include <iostream>
using namespace std;

void EnterMineAndDigForGold::Enter(Miner* target)
{
	if (target->GetLocation() != goldmine)
	{
	//cout << "\n" << target->GetID() << "walking to goldmine";
		target->ChangeLocation(goldmine);
	}
}
void EnterMineAndDigForGold::Execute(Miner * target)
{
	int n = rand() % 50;

	target->AddGold(n);
	wcout << L"\n" << GetNameOfEntity(target->GetID()) << L": pick up" << n << L"gold";

	target->LoseEnergy(n);
	target->LoseWater(n);

	if (target->IsPocketFull())
	{
		target->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
	}
	else if (target->IsThirsty())
	{
		target->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
	else if (target->IsTired())
	{
		target->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	}
}
void EnterMineAndDigForGold::Exit(Miner* target)
{
	//cout << "\n" << GetNameOfEntity(target->GetID()) << ": leave goldmine";
}


bool EnterMineAndDigForGold::OnMessage(Miner*, const Telegram&)
{
	return false;
}
void VisitBankAndDepositGold::Enter(Miner* target)
{
	if (target->GetLocation() != bank)
	{
		target->ChangeLocation(bank);
	}
}
void VisitBankAndDepositGold::Execute(Miner* target)
{
	target->DepositGold(100);
	wcout << L"\n" << GetNameOfEntity(target->GetID()) << L": deposit gold";
	target->GetFSM()->ChangeState(EnterMineAndDigForGold::Instance());
	/*if (target->IsThirsty())
	{
	target->ChangeState(new QuenchThirst());
	}
	else if (target->IsTired())
	{
	target->ChangeState(new GoHomeAndSleepTilRested());
	}
	*/
}
void VisitBankAndDepositGold::Exit(Miner*)
{

}

bool VisitBankAndDepositGold::OnMessage(Miner*, const Telegram&)
{
	return false;
}

void GoHomeAndSleepTilRested::Enter(Miner* target)
{
	if (target->GetLocation() != shack)
	{
		target->ChangeLocation(shack);
		wcout << L"\n" << GetNameOfEntity(target->GetID()) << L": honey i'm home";
		MessageDispatcher::Instance()->DispatchMessage(0, target->GetID(), ent_Elsa, Msg_HiHoneyImHome, NULL);

	}
}

void GoHomeAndSleepTilRested::Execute(Miner* target)
{
	target->Recover();
	wcout << L"\n" << GetNameOfEntity(target->GetID()) << L": zzZ";
	if (target->IsFullEnergy())
	{
		target->GetFSM()->ChangeState(EnterMineAndDigForGold::Instance());
	}
	else if (target->IsThirsty())
	{
		target->GetFSM()->ChangeState(QuenchThirst::Instance());
	}
}
void GoHomeAndSleepTilRested::Exit(Miner*)
{

}

bool GoHomeAndSleepTilRested::OnMessage(Miner* target, const Telegram& telegram)
{
	switch (telegram.Msg)
	{
	case Msg_StewReady:
	{
		wcout << L"\n" << GetNameOfEntity(target->GetID()) << L": comming";
		target->GetFSM()->ChangeState(EatStew::Instance());
	}
		return true;
	default:
		break;
	}
	return false;
}

void QuenchThirst::Enter(Miner*)
{

}
void QuenchThirst::Execute(Miner* target)
{
	target->DrinkWater(10);
	wcout << L"\n" << GetNameOfEntity(target->GetID()) << L": quench thirsting";
	if (target->IsFullWater())
	{
		target->GetFSM()->ChangeState(EnterMineAndDigForGold::Instance());
	}
}
void QuenchThirst::Exit(Miner*)
{

}

bool QuenchThirst::OnMessage(Miner*, const Telegram&)
{
	return false;
}

void EatStew::Enter(Miner*)
{

}

void EatStew::Execute(Miner* target)
{
	wcout << L"\n" << GetNameOfEntity(target->GetID()) << L": Tastes real good too!";

	target->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner*)
{

}

bool EatStew::OnMessage(Miner*, const Telegram&)
{
	return false;
}