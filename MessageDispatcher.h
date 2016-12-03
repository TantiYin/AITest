///////////////////////////////////////////////////////////////////////
//消息管理类
//负责分发消息，接收消息
///////////////////////////////////////////////////////////////////////

#pragma once

#include "telegram.h"
#include <set>

class BaseGameEntity;

class MessageDispatcher
{
private:
	std::set<Telegram> priorityQ;

	void discharge(BaseGameEntity* pReceiver, const Telegram & msg);
	MessageDispatcher(){}
public:
	static MessageDispatcher* Instance();

	void DispatchMessage(int delay,
		int sender,
		int receiver,
		int msg,
		void * extraInfo);
	void DispatchDelayedMessage();


};

