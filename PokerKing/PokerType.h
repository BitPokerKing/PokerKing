#pragma once

#include <cstring>

enum PokerColor
{
	SPADES, HEARTS, CLUBS, DIAMONDS		//黑桃、红桃、梅花、方片
};

class PokerType
{
public:
	char num[3];	//点数例如J,10	
	PokerColor color;	//花色

	PokerType();

	inline bool operator==(const PokerType& x)
	{
		return strcmp(num, x.num)==0 && color==x.color;
	}

	void setCard(int i);
	void setCard(int i, PokerColor c);

	void fromInput();

	bool isAce();

	//将扑克转换为Int型大小,对A分两种情况转换
	int toInt(bool ace);

};

