#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <cstring>


enum PokerColor
{
	SPADES, HEARTS, CLUBS, DIAMONDS		//���ҡ����ҡ�÷������Ƭ
};

class PokerType
{
public:
	char num;	//��������J,10,����10��X��ʾ
	PokerColor color;	//��ɫ

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

	//���˿�ת��ΪInt�ʹ�С,��A���������ת��
	int toInt(bool ace);

};

