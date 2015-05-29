#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define max(a,b) a>b?a:b;

#include "PokerType.h"
#include "PairPoker.h"
#include "MCTS.h"

//检查是否存在Ace
void Cards::checkAexist()
{
	for (int i = 0; i < 7; i++)
	{
		if (i < 2)
		{
			if (TwoPok[i].isAce())
			{
				existA = true;
				return;
			}
		}
		else
		{
			if (PubPok[i - 2].isAce())
			{
				existA = true;
				return;
			}
		}
	}
}

//计算当前7张牌中5张最大组成牌的分数
void Cards::calMaxScore()
{
	score = 0;
	Maxs = 0;
	checkAexist();
	bool Flush = false;
	bool Straight = false;
	bool S_F = false;
	calpoint(1);
	sortpoint();
	score = checkSF();
	Maxs = max(Maxs, score);
	score = checkOther();
	Maxs = max(Maxs, score);
	if (existA)
	{
		calpoint(0);
		sortpoint();
		score = checkSF();
		Maxs = max(Maxs, score);
	}
}

//将7张牌点数转为Int并且记录花色数和点数个数
void Cards::calpoint(int Atype)
{
	maxSum = 0;
	maxPoint = 0;
	memset(colorSum, 0, sizeof(colorSum));
	memset(pointSum, 0, sizeof(pointSum));
	for (int i = 0; i < 7; i++)
	{
		if (i < 2)
		{
			point[i] = TwoPok[i].toInt(Atype);
			color[i] = TwoPok[i].color;
			colorSum[TwoPok[i].color]++;
		}
		else
		{
			point[i] = PubPok[i - 2].toInt(Atype);
			color[i] = PubPok[i - 2].color;
			colorSum[PubPok[i - 2].color]++;
		}
		pointSum[point[i]]++;
		if (pointSum[point[i]] > maxSum)
		{
			maxSum = pointSum[point[i]];
			maxPoint = point[i];
		}
		else if (pointSum[point[i]] == maxSum)
		{
			if (maxPoint < point[i])
			{
				maxPoint = point[i];
			}
		}
	}
}

//将7张牌点数从大到小排序
void Cards::sortpoint()
{
	int temp;
	int tempcolor;
	for (int i = 0; i < 7; i++)
	{
		for (int j = i + 1; j < 7; j++)
		{
			if (point[i] < point[j])
			{
				temp = point[i];
				point[i] = point[j];
				point[j] = temp;
				tempcolor = color[i];
				color[i] = color[j];
				color[j] = tempcolor;
			}
		}
	}
}

//检查是否为同花顺以及同花和顺子
long long int Cards::checkSF()
{
	long long int Maxscore = 0, nowscore = 0;
	bool F = true;//同花	
	bool S = false;//顺子
	int i, j, k, l;
	int samecolor = 0, lastcolor;//配合检查是否为同花顺
	for (int i = 0; i < 4; i++)
	{
		if (colorSum[i] >= 5)	//确定同花的颜色
		{
			samecolor = i;
			break;
		}
	}
	for (i = 0; i < 3; i++)
	{
		F = true;
		S = true;
		j = i;	//j为上一张牌
		lastcolor = color[j];
		k = j + 1;	//k为当前牌，检查顺子，不能出现k点数与j点数相等
		for (l = 1; l < 5 && k < 7; l++)
		{
			while (point[k] == point[j]) //当相邻牌点数相同则跳至下一张
			{
				if (lastcolor != samecolor && color[k] == samecolor) //如果与j点数相同的牌中有同花花色，将上次的颜色也改为同花颜色
				{
					lastcolor = samecolor;
				}
				k++;
			}
			if (k >= 7)	//如果由于有相同的牌导致k下移使得牌不够，也无法成顺
			{
				S = false;
				break;
			}
			if (F && samecolor == lastcolor);
			else
			{
				F = false;
			}
			if (point[j] - point[k]>1)	//如果有点数差超过1的则这5张牌无法顺子
			{
				S = false;
				break;
			}
			j = k;
			lastcolor = color[j];
			k++;
		}
		//还得对第五张牌可能相同的情况进行同花检查
		while (point[k] == point[j] && k < 7)
		{
			if (lastcolor != samecolor && color[k] == samecolor) //如果与j点数相同的牌中有同花花色，将上次的颜色也改为同花颜色
			{
				lastcolor = samecolor;
			}
			k++;
		}
		if (F && samecolor == lastcolor);
		else
		{
			F = false;
		}
		if (l == 5)
		{
			if (F == true)		//同花顺首位得分为8,其他牌型一次递减
			{
				nowscore = 0;
				nowscore += 8 * pow(10.0, 10);
				nowscore += (point[i] * pow(10.0, 8));
				j = i;	//j为上一张牌
				k = j + 1;	//k为当前牌，检查顺子，不能出现k点数与j点数相等
				for (l = 1; l < 5 && k < 7; l++)
				{
					while (point[k] == point[j]) //当相邻牌点数相同则跳至下一张
					{
						k++;
					}
					if (F && color[k] == color[j]);
					else
					{
						F = false;
					}
					if (point[j] - point[k]>1)	//如果有点数差超过1的则这5张牌无法顺子
					{
						S = false;
						break;
					}
					nowscore += (point[k] * pow(10.0, 8 - 2 * l));
					j = k;
					k++;
				}
				return nowscore;
			}
			else
			{
				//顺子不同花的分数
				nowscore = 0;
				nowscore += 4 * pow(10.0, 10);
				nowscore += (point[i] * pow(10.0, 8));
				j = i;	//j为上一张牌
				k = j + 1;	//k为当前牌，检查顺子，不能出现k点数与j点数相等
				for (l = 1; l < 5 && k < 7; l++)
				{
					while (point[k] == point[j]) //当相邻牌点数相同则跳至下一张
					{
						k++;
					}
					if (F && color[k] == point[j]);
					else
					{
						F = false;
					}
					if (point[j] - point[k]>1)	//如果有点数差超过1的则这5张牌无法顺子
					{
						S = false;
						break;
					}
					nowscore += (point[k] * pow(10.0, 8 - 2 * l));
					j = k;
					k++;
				}
				Maxscore = max(Maxscore, nowscore);
			}
		}
	}
	//检查同花
	for (i = 0; i < 4; i++)
	{
		if (colorSum[i] >= 5)
		{
			nowscore = 0;
			nowscore += (5 * pow(10.0, 10));
			k = 0;
			for (j = 0; j < 7 && k < 5; j++)
			{
				if (color[j] == i)
				{
					nowscore += (point[j] * pow(10.0, 8 - 2 * k));
					k++;
				}
			}
			break;
		}
	}
	Maxscore = max(Maxscore, nowscore);
	return Maxscore;
}

//计算其他多张同点数的牌型分数
long long int Cards::checkOther()
{
	long long int nowscore = 0;
	//计算四条的分数
	if (maxSum == 4)
	{
		nowscore += 7 * pow(10.0, 10);
		nowscore += maxPoint*pow(10.0, 8);
		for (int i = 0; i < 7; i++)
		{
			if (point[i] != maxPoint)
			{
				nowscore += (point[i]);
				break;
			}
		}
	}
	//计算葫芦或三条的分数
	else if (maxSum == 3)
	{
		for (int i = 14; i > 0; i--)
		{
			//葫芦计算分数
			if (pointSum[i] == 2)
			{
				nowscore += 6 * pow(10.0, 10);
				nowscore += (maxPoint*pow(10.0, 8));
				nowscore += i;
				return nowscore;
			}
		}
		//三条计算分数
		int k = 0;
		nowscore += 3 * pow(10.0, 10);
		nowscore += (maxPoint*pow(10.0, 8));
		for (int i = 0; i < 7 && k<2; i++)
		{
			if (point[i] != maxPoint)
			{
				nowscore += (point[i] * pow(10.0, 2 * (1 - k)));
				k++;
			}
		}
	}
	//计算两对或一对分数
	else if (maxSum == 2)
	{
		//两对分数

		for (int i = 14; i > 0; i--)
		{
			if (pointSum[i] == 2 && i != maxPoint)
			{
				nowscore += 2 * pow(10.0, 10);
				nowscore += (maxPoint*pow(10.0, 8));
				nowscore += (i*pow(10.0, 4));
				for (int j = 0; j < 7; j++)
				{
					if (point[j] != i && point[j] != maxPoint)
					{
						nowscore += point[j];
						return nowscore;
					}
				}
			}
		}
		//一对分数
		nowscore += 1 * pow(10.0, 10);
		nowscore += (maxPoint*pow(10.0, 8));
		int k = 0;
		for (int i = 0; i < 7 && k < 3; i++)
		{
			if (point[i] != maxPoint)
			{
				nowscore += (point[i] * pow(10.0, 4 - 2 * k));
				k++;
			}
		}
	}
	//高牌分数
	else if (maxSum == 1)
	{
		for (int i = 0; i < 5; i++)
		{
			nowscore += (point[i] * pow(10.0, 8 - 2 * i));
		}
	}
	return nowscore;
}


//蒙特卡洛模拟
void MCTS::MonteCarlo(int t)
{
	while (t--)
	{
		copyPoker();
		if (state == 0)
		{
			Hold[0] = APoker.randPok();
			Hold[1] = APoker.randPok();
			sortHold();
			root = &first[Hold[0].toInt(1)][Hold[1].toInt(1)][Hold[0].color][Hold[1].color];
			opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();
			opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();
			opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();

			calMyScore();
			for (int i = 1; i < playerSum; i++)
			{
				otherHold[i][0] = opPoker.randPok();
				otherHold[i][1] = opPoker.randPok();
				calOtherScore(i);
				if (myscore < otherscore)
				{
					root->visit++;
					break;
				}
				if (i == playerSum - 1)
				{
					root->visit++;
					root->win++;
				}
			}
		}
		else if (state == 1)
		{
			sortHold();
			root = &first[Hold[0].toInt(1)][Hold[1].toInt(1)][Hold[0].color][Hold[1].color];
			opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();
			opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();
			opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();

			calMyScore();
			for (int i = 1; i < playerSum; i++)
			{
				otherHold[i][0] = opPoker.randPok();
				otherHold[i][1] = opPoker.randPok();
				calOtherScore(i);
				calOtherScore(i);
				if (myscore < otherscore)
				{
					root->visit++;
					break;
				}
				if (i == playerSum - 1)
				{
					root->visit++;
					root->win++;
				}
			}
		}
		else if (state == 2)
		{
			opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();
			opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();

			calMyScore();
			for (int i = 1; i < playerSum; i++)
			{
				otherHold[i][0] = opPoker.randPok();
				otherHold[i][1] = opPoker.randPok();
				calOtherScore(i);
				if (myscore < otherscore)
				{
					visit++;
					calMyScore();
					break;
				}
				if (i == playerSum - 1)
				{
					visit++;
					win++;
				}
			}
		}
		else if (state == 3)
		{
			opPoker.randPok();
			opPub[opPubLen++] = opPoker.randPok();

			calMyScore();
			for (int i = 1; i < playerSum; i++)
			{
				otherHold[i][0] = opPoker.randPok();
				otherHold[i][1] = opPoker.randPok();
				calOtherScore(i);
				if (myscore < otherscore)
				{
					visit++;
					break;
				}
				if (i == playerSum - 1)
				{
					visit++;
					win++;
				}
			}
		}
		else if (state == 4)
		{
			calMyScore();
			for (int i = 1; i < playerSum; i++)
			{
				otherHold[i][0] = opPoker.randPok();
				otherHold[i][1] = opPoker.randPok();
				calOtherScore(i);
				if (myscore < otherscore)
				{
					visit++;
					break;
				}
				if (i == playerSum - 1)
				{
					visit++;
					win++;
				}
			}
		}
	}
}

//整理底牌
void MCTS::sortHold()
{
	PokerType temp;
	int a = Hold[0].toInt(1);
	int b = Hold[1].toInt(1);
	temp = Hold[0];
	if (a < b)
	{
		Hold[0] = Hold[1];
		Hold[1] = temp;
	}
	else if (a == b)
	{
		if (Hold[0].color>Hold[1].color)
		{
			Hold[0] = Hold[1];
			Hold[1] = temp;
		}
	}
}

//恢复模拟用扑克
void MCTS::copyPoker()
{
	mtx.lock();
	memcpy(opPub, Pub, sizeof(Pub));
	memcpy(opHold, Hold, sizeof(Hold));
	opHoldLen = HoldLen;
	opPubLen = PubLen;
	memcpy(opPoker.All, APoker.All, sizeof(APoker.All));
	opPoker.len = APoker.len;
	mtx.unlock();
}

//计算我方分数
void MCTS::calMyScore()
{
	for (int i = 0; i < 7; i++)
	{
		if (i < 2)
		{
			myCards.TwoPok[i] = Hold[i];
		}
		else
		{
			myCards.PubPok[i - 2] = opPub[i - 2];
		}
	}
	myCards.calMaxScore();
	myscore = myCards.Maxs;
}

//计算其他人分数
void MCTS::calOtherScore(int n)
{
	for (int i = 0; i < 7; i++)
	{
		if (i < 2)
		{
			enemyCards.TwoPok[i] = otherHold[n][i];
		}
		else
		{
			enemyCards.PubPok[i - 2] = opPub[i - 2];
		}
	}
	enemyCards.calMaxScore();
	otherscore = enemyCards.Maxs;
}

//初始化底牌及公牌
void MCTS::initPok()
{
	HoldLen = 0;
	PubLen = 0;
}

//设置底牌
void MCTS::setHold(PokerType pok)
{
	mtx.lock();
	Hold[HoldLen++] = pok;
	mtx.unlock();
}

//设置公牌
void MCTS::setPub(PokerType pok)
{
	mtx.lock();
	Pub[PubLen++] = pok;
	mtx.unlock();
}


//获取当前胜率
double MCTS::getRate()
{
	return visit==0 ? 0.0 : (double)win/visit;
}

//清空胜率
void MCTS::resetRate()
{
	visit=0;
	win=0;
}

//在state==0或1时不清空胜率
void MCTS::tryResetRate()
{
	if(state>1)
	{
		resetRate();
	}
}

//线程主函数
void MCTS::threadMC(MCTS* inst)
{
	while(!inst->threadEnded)
	{
		inst->MonteCarlo(1);
	}
}

//开始线程
void MCTS::beginThread()
{
	threadEnded = false;
	threadExpand = new thread(threadMC, this);
}

//结束线程
void MCTS::endThread()
{
	threadEnded = true;
	threadExpand->join();
}
