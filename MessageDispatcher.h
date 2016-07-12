///////////////////////////////////////////////////////////////////////
//��Ϣ������
//����ַ���Ϣ��������Ϣ
///////////////////////////////////////////////////////////////////////

#ifndef _MESSAGE_DISPATCHER_H_
#define _MESSAGE_DISPATCHER_H_

#include "npc.h"
#include "telegram.h"
#include <set>

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
#endif