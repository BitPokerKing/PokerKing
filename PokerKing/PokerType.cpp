#include "PokerType.h"

#include <cstring>
#include <cstdio>

char InitPoker[] = "A23456789XJQK";
char PokerName[13][3] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
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
	num=InitPoker[i % 13];
	color = c;
}

void PokerType::fromInput()
{
	static char s[20];
	scanf("%s", s);
	num=s[0];
	if(num=='1')
		num='X';
	scanf("%d", &color);
}

void PokerType::print()
{
	printf("%s %d\n", num, color);
}

bool PokerType::isAce()
{
	return num==0;
}

int PokerType::toInt(bool ace)
{
	if (num >= '2' && num <= '9')
	{
		return num - '0';
	}
	else if (num == 'J')
	{
		return 11;
	}
	else if (num == 'Q')
	{
		return 12;
	}
	else if (num == 'K')
	{
		return 13;
	}
	else if (num == 'X')
	{
		return 10;
	}
	else if (num == 'A')
	{
		if (ace)
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

