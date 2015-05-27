#include "PokerType.h"

#include <cstring>

char InitPoker[13][3] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
PokerColor ColorList[4]={ SPADES, HEARTS, CLUBS, DIAMONDS };

PokerType::PokerType()
{
	setCard(0);
}

void PokerType::setCard(int i)
{
	setCard(i, ColorList[i/13%4]);
}

void PokerType::setCard(int i, PokerColor c)
{
	strcpy(num, InitPoker[i % 13]);
	color = c;
}

int PokerType::toInt(bool ace)
{
	if (num[0] >= '2' && num[0] <= '9')
	{
		return num[0] - '0';
	}
	else if (num[0] == 'J')
	{
		return 11;
	}
	else if (num[0] == 'Q')
	{
		return 12;
	}
	else if (num[0] == 'K')
	{
		return 13;
	}
	else if (num[0] == '1' && num[1] == '0')
	{
		return 10;
	}
	else if (num[0] == 'A')
	{
		if (A)
		{
			return 14;
		}
		else
		{
			return 1;
		}
	}
	return -1;
}

