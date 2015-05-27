#pragma once

#include "PokerType.h"

//一整副牌
class PairPoker
{
public:
	int len;
	PokerType All[52];

	PairPoker();

	//初始化整副牌
	void init();

	//随机抽取一张牌，可用于发牌和销牌
	PokerType randPok();

	//销毁指定牌
	void destroy(PokerType& t);

};


