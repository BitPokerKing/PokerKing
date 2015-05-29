#ifndef MCTS_H
#define MCTS_H

#define _CRT_SECURE_NO_WARNINGS
#include "PokerType.h"
#include "PairPoker.h"

#include <thread>
#include <mutex>
using namespace std;


class CardsBase
{
public:
	PokerType TwoPok[2];	//两张底牌
	PokerType PubPok[5];	//5张公牌
};

class node : public CardsBase
{
public:
	short int nowPub;			//现有公牌张数
	double winrate;
	int win;
	int visit;
	node *child;
	node *next;
	node *parent;
};

class Cards : public CardsBase
{
public:
	bool existA;			//是否存在Ace
	long long int score = 0, Maxs;

	//计算当前7张牌中5张最大组成牌的分数
	void calMaxScore();

private:
	short int point[7];			//7张牌的点数
	short int color[7];			//7张牌的花色
	short int colorSum[4];		//各颜色的总数
	short int pointSum[15];		//各点数的总数
	short int maxSum = 0;		//记录一对或者三条四条
	short int maxPoint = 0;		//记录点数出现最频繁且最大的那张牌的点数

	//检查是否存在Ace
	void checkAexist();

	//将7张牌点数转为Int并且记录花色数和点数个数
	void calpoint(int Atype);

	//将7张牌点数从大到小排序
	void sortpoint();

	//检查是否为同花顺以及同花和顺子
	long long int checkSF();

	//计算其他多张同点数的牌型分数
	long long int checkOther();
};

class MCTS
{
public:
	PairPoker APoker;	//实际扑克
	PairPoker opPoker;	//模拟用扑克
	PokerType Hold[2];	//我方底牌
	PokerType opHold[2];//模拟时我方底牌
	PokerType Pub[5];	//公牌
	PokerType opPub[5];	//模拟公牌
	int HoldLen = 0, opHoldLen;
	int PubLen = 0;
	int opPubLen;			//模拟用公牌张数
	node first[15][15][4][4];	//保存底牌的所有情况
	node *root;
	short int state;	//共5种模拟阶段：未拿到底牌，拿到底牌，3张公牌，4张，5张
	int playerSum = 8;			//总玩家数
	PokerType otherHold[8][2];	//其他人底牌
	Cards myCards, enemyCards;	//我方和其他人分数比较
	long long int myscore, otherscore;
	int win = 0, visit = 0;//2,3,4阶段的胜负情况

	mutex mtx; //用于协调扩展线程和获取结果过程的互斥锁
	thread* threadExpand; //离线扩展线程
	bool threadEnded; //标记扩展是否结束

	//蒙特卡洛模拟
	void MonteCarlo(int t=10000);

	//设置底牌
	void setHold(PokerType pok);

	//设置公牌
	void setPub(PokerType pok);

	//初始化公牌
	void initPok();

	//获取当前胜率
	double getRate();

	//清空胜率
	void resetRate();

	//在state==0或1时不清空胜率
	void tryResetRate();

private:

	//线程主函数
	static void threadMC(MCTS* inst);

	//开始线程
	void beginThread();

	//结束线程
	void endThread();

	//整理底牌
	void sortHold();

	//恢复模拟用扑克
	void copyPoker();

	//计算我方分数
	void calMyScore();

	//计算其他人分数
	void calOtherScore(int n);

	
};
#endif
