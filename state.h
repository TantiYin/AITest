#ifndef _STATE_H_
#define _STATE_H_

template<class entityType>
class State
{
public:
	virtual void Enter(entityType*) = 0;
	virtual void Execute(entityType*) = 0;
	virtual void Exit(entityType*) = 0;

	virtual bool OnMessage(entityType*, const Telegram&) = 0;

private:

};

#endif