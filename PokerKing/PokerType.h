#pragma once

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

	void setCard(int i);
	void setCard(int i, PokerColor c);

	//���˿�ת��ΪInt�ʹ�С,��A���������ת��
	int toInt(bool ace);

};

