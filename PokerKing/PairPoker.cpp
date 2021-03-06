#include "PairPoker.h"

#include <cstdlib>
#include <cstring>

PairPoker::PairPoker()
{
	init();
}

void PairPoker::init()
{
	len = 52;
	for (int i = 0; i < 52; i++)
	{
		All[i].setCard(i);
	}
}
		
PokerType PairPoker::randPok()
{
	int num = rand() % len;
	PokerType temp;
	temp = All[num];
	All[num] = All[len - 1];
	len--;
	return temp;
}

void PairPoker::destroy(PokerType& t)
{
	for (int i = 0; i < len; i++)
	{
		if (t == All[i])
		{
			All[i] = All[len-1];
			len = len - 1;
			break;
		}
	}
}

