#pragma once
#define _CRT_SECURE_NO_WARNINGS
enum PokerColor
{
	SPADES, HEARTS, CLUBS, DIAMONDS		//���ҡ����ҡ�÷������Ƭ
};

char PokColor[4][10] = { "SPADES", "HEARTS", "CLUBS", "DIAMONDS" };

class PokerType
{
public:
	char num[3];	//��������J,10	
	PokerColor color;	//��ɫ

	PokerType();

	void setCard(int i);
	void setCard(int i, PokerColor c);

	//���˿�ת��ΪInt�ʹ�С,��A���������ת��
	int toInt(bool ace);

};

