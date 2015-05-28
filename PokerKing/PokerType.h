#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <cstring>


enum PokerColor
{
	SPADES, HEARTS, CLUBS, DIAMONDS		//黑桃、红桃、梅花、方片
};

class PokerType
{
public:
	char num;	//点数例如J,10,其中10用X表示
	PokerColor color;	//花色

	PokerType();

	inline bool operator==(const PokerType& x)
	{
		return num==x.num && color==x.color;
	}

	void setCard(int i);
	void setCard(int i, PokerColor c);

	void fromInput();
	void print();

	bool isAce();

	//将扑克转换为Int型大小,对A分两种情况转换
	int toInt(bool ace);

};

