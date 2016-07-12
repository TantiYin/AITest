#include "headers.h"
#include <time.h>

MessageDispatcher* MessageDispatcher::Instance()
{
	static MessageDispatcher* ins = new MessageDispatcher();
	return ins;
}

void MessageDispatcher::discharge(BaseGameEntity* pReceiver, const Telegram & msg)
{
	pReceiver->HandleMessage(msg);
}

void MessageDispatcher::DispatchMessage(int delay, int sender, int receiver, int msg, void * extraInfo)
{
	BaseGameEntity* pReciever = EntityManager::Instance()->GetEntityFromID(receiver);
	Telegram telegram( sender, receiver, msg, delay, extraInfo);

	if (delay <= 0)
	{
		discharge(pReciever, telegram);
	}
	else
	{
		time_t currentTime = time(0);
		telegram.DispatchTime = currentTime + delay;
		priorityQ.insert(telegram);
	}

}

void MessageDispatcher::DispatchDelayedMessage()
{
	time_t currentTime = time(0);
	
	if (!priorityQ.empty() && 
		priorityQ.begin()->DispatchTime < currentTime && 
		priorityQ.begin()->DispatchTime > 0)
	{
		Telegram telegram = *(priorityQ.begin());
		BaseGameEntity * reciever = EntityManager::Instance()->GetEntityFromID(telegram.Receiver);
		discharge(reciever, telegram);
		priorityQ.erase(priorityQ.begin());
	}
}