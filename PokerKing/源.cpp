#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#define max(a,b) a>b?a:b;

char InitPoker[13][3] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

#include "PokerType.h"
#include "PairPoker.h"

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

//最终计算分数阶段的7张牌
class Cards : public CardsBase
{
public:
	bool existA;			//是否存在Ace
	long long int score = 0, Maxs;
	//计算当前7张牌中5张最大组成牌的分数
	void calMaxScore()
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

private:
	short int point[7];			//7张牌的点数
	short int color[7];			//7张牌的花色
	short int colorSum[4];		//各颜色的总数
	short int pointSum[15];		//各点数的总数
	short int maxSum = 0;		//记录一对或者三条四条
	short int maxPoint = 0;		//记录点数出现最频繁且最大的那张牌的点数
	
	void checkAexist()
	{
		for (int i = 0; i < 7; i++)
		{
			if (i < 2)
			{
				if (strcmp(TwoPok[i].num, "A") == 0)
				{
					existA = true;
					return;
				}
			}
			else
			{
				if (strcmp(PubPok[i-2].num, "A") == 0)
				{
					existA = true;
					return;
				}
			}
		}
	}

	//将7张牌点数转为Int并且记录花色数和点数个数
	void calpoint(int Atype)
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
	void sortpoint()
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
	long long int checkSF()
	{
		long long int Maxscore = 0, nowscore = 0;
		bool F = true;//同花	
		bool S = false;//顺子
		int i, j, k, l;
		for (i = 0; i < 3; i++)
		{
			F = true;
			S = true;
			j = i;	//j为上一张牌
			k = j + 1;	//k为当前牌，检查顺子，不能出现k点数与j点数相等
			for (l = 1; l < 5 && k < 7; l++)
			{
				while (point[k] == point[j]) //当相邻牌点数相同则跳至下一张
				{
					k++;
				}
				if (k >= 7)	//如果由于有相同的牌导致k下移使得牌不够，也无法成顺
				{
					S = false;
					break;
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
				j = k;
				k++;
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
	long long int checkOther()
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

};


class MCTS
{
public:
	PairPoker APoker;	//实际扑克
	PairPoker opPoker;	//模拟用扑克
	PokerType Hold[2];	//我方底牌
	PokerType Pub[5];	//公牌
	PokerType opPub[5];	//模拟公牌
	int PubLen = 0;
	int opPubLen;			//模拟用公牌张数
	struct node first[15][15][4][4];	//保存底牌的所有情况
	struct node *root;
	short int state;	//共5种模拟阶段：未拿到底牌，拿到底牌，3张公牌，4张，5张
	int playerSum = 8;			//总玩家数
	PokerType otherHold[8][2];	//其他人底牌
	Cards myCards, enemyCards;	//我方和其他人分数比较
	long long int myscore, otherscore;
	int win = 0, visit = 0;//2,3,4阶段的胜负情况
	//蒙特卡洛模拟
	void MonteCarlo()
	{
		int t = 10000;
		while (t--)
		{
			opPubLen = PubLen;
			if (state == 0)
			{
				copyPoker();
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
				copyPoker();
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
				copyPoker();
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
				copyPoker();
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
			else if (state==4)
			{
				copyPoker();
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

private:
	//整理底牌
	void sortHold()
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
	void copyPoker()
	{
		memcpy(opPoker.All, APoker.All, sizeof(APoker.All));
		opPoker.len = APoker.len;
	}

	//计算我方分数
	void calMyScore()
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
	void calOtherScore(int n)
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
};
int main()
{
	srand((unsigned)time(NULL));
	clock_t start, end;
	MCTS myTree;
	myTree.APoker.Init();
	while (1)
	{
		for (int i = 0; i < 2; i++)
		{
			scanf("%s", myTree.Hold[i].num);
			scanf("%d", &myTree.Hold[i].color);
		}
		myTree.APoker.destroy(myTree.Hold[0].num, myTree.Hold[0].color);
		myTree.APoker.destroy(myTree.Hold[1].num, myTree.Hold[1].color);
		myTree.state = 1;

		scanf("%s %d", myTree.Pub[0].num, &myTree.Pub[0].color);
		scanf("%s %d", myTree.Pub[1].num, &myTree.Pub[1].color);
		scanf("%s %d", myTree.Pub[2].num, &myTree.Pub[2].color);
		myTree.PubLen = 3;
		myTree.state = 2;
		start = clock();
		myTree.MonteCarlo();

		if (myTree.state==1)
			printf("%lf\n", 1.0*myTree.root->win / myTree.root->visit);
		else
		{
			printf("%lf\n", 1.0*myTree.win / myTree.visit);
		}

		end = clock();
		printf("%d ms\n", end - start);
	}
	return 0;
}