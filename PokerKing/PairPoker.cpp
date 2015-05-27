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

void PairPoker::destroy(char *point, int color)
{
	for (int i = 0; i < len; i++)
	{
		if (strcmp(point, All[i].num)==0 && color == All[i].color)
		{
			All[i] = All[len-1];
			len = len - 1;
			break;
		}
	}
}
