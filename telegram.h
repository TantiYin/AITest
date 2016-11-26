#pragma once


#include <stdio.h>
#include <math.h>
#include <iostream>

struct Telegram
{
	int Sender;
	int Receiver;
	int Msg;
	time_t DispatchTime;
	void * extraInfo;

	Telegram() :Sender(-1),
		Receiver(-1),
		Msg(-1),
		DispatchTime(-1),
		extraInfo(NULL)
	{}

	Telegram(int s, int r, int m, int t, void* e) :Sender(s), Receiver(r), Msg(m), DispatchTime(t), extraInfo(e)
	{}

};

//these telegrams will be stored in a priority queue. Therefore the >
//operator needs to be overloaded so that the PQ can sort the telegrams
//by time priority. Note how the times must be smaller than
//SmallestDelay apart before two Telegrams are considered unique.
const double SmallestDelay = 2;


inline bool operator==(const Telegram& t1, const Telegram& t2)
{
	return (abs(t1.DispatchTime - t2.DispatchTime) < SmallestDelay) &&
		(t1.Sender == t2.Sender) &&
		(t1.Receiver == t2.Receiver) &&
		(t1.Msg == t2.Msg);
}

inline bool operator<(const Telegram& t1, const Telegram& t2)
{
	if (t1 == t2)
	{
		return false;
	}

	else
	{
		return  (t1.DispatchTime < t2.DispatchTime);
	}
}

inline std::ostream& operator<<(std::ostream& os, const Telegram& t)
{
	os << "time: " << t.DispatchTime << "  Sender: " << t.Sender
		<< "   Receiver: " << t.Receiver << "   Msg: " << t.Msg;

	return os;
}

//handy helper function for dereferencing the ExtraInfo field of the Telegram 
//to the required type.
template <class T>
inline T DereferenceToType(void* p)
{
	return *(T*)(p);
}

