#pragma once

#include <cstring>

enum PokerColor
{
	SPADES, HEARTS, CLUBS, DIAMONDS		//���ҡ����ҡ�÷������Ƭ
};

class PokerType
{
public:
	char num[3];	//��������J,10	
	PokerColor color;	//��ɫ

	PokerType();

	inline bool operator==(const PokerType& x)
	{
		return strcmp(num, x.num)==0 && color==x.color;
	}

	void setCard(int i);
	void setCard(int i, PokerColor c);

	void fromInput();

	bool isAce();

	//���˿�ת��ΪInt�ʹ�С,��A���������ת��
	int toInt(bool ace);

};

