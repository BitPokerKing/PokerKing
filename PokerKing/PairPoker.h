#pragma once

#include "PokerType.h"

//һ������
class PairPoker
{
public:
	int len;
	PokerType All[52];

	PairPoker();

	//��ʼ��������
	void init();

	//�����ȡһ���ƣ������ڷ��ƺ�����
	PokerType randPok();

	//����ָ����
	void destroy(PokerType& t);

};


