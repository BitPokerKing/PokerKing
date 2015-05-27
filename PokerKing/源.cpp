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
	PokerType TwoPok[2];	//���ŵ���
	PokerType PubPok[5];	//5�Ź���
};

class node : public CardsBase
{
public:
	short int nowPub;			//���й�������
	double winrate;
	int win;
	int visit;
	node *child;
	node *next;
	node *parent;
};

//���ռ�������׶ε�7����
class Cards : public CardsBase
{
public:
	bool existA;			//�Ƿ����Ace
	long long int score = 0, Maxs;
	//���㵱ǰ7������5���������Ƶķ���
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
	short int point[7];			//7���Ƶĵ���
	short int color[7];			//7���ƵĻ�ɫ
	short int colorSum[4];		//����ɫ������
	short int pointSum[15];		//������������
	short int maxSum = 0;		//��¼һ�Ի�����������
	short int maxPoint = 0;		//��¼����������Ƶ�������������Ƶĵ���
	
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

	//��7���Ƶ���תΪInt���Ҽ�¼��ɫ���͵�������
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

	//��7���Ƶ����Ӵ�С����
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

	//����Ƿ�Ϊͬ��˳�Լ�ͬ����˳��
	long long int checkSF()
	{
		long long int Maxscore = 0, nowscore = 0;
		bool F = true;//ͬ��	
		bool S = false;//˳��
		int i, j, k, l;
		for (i = 0; i < 3; i++)
		{
			F = true;
			S = true;
			j = i;	//jΪ��һ����
			k = j + 1;	//kΪ��ǰ�ƣ����˳�ӣ����ܳ���k������j�������
			for (l = 1; l < 5 && k < 7; l++)
			{
				while (point[k] == point[j]) //�������Ƶ�����ͬ��������һ��
				{
					k++;
				}
				if (k >= 7)	//�����������ͬ���Ƶ���k����ʹ���Ʋ�����Ҳ�޷���˳
				{
					S = false;
					break;
				}
				if (F && color[k] == color[j]);
				else
				{
					F = false;
				}
				if (point[j] - point[k]>1)	//����е������1������5�����޷�˳��
				{
					S = false;
					break;
				}
				j = k;
				k++;
			}
			if (l == 5)
			{
				if (F == true)		//ͬ��˳��λ�÷�Ϊ8,��������һ�εݼ�
				{
					nowscore = 0;
					nowscore += 8 * pow(10.0, 10);
					nowscore += (point[i] * pow(10.0, 8));
					j = i;	//jΪ��һ����
					k = j + 1;	//kΪ��ǰ�ƣ����˳�ӣ����ܳ���k������j�������
					for (l = 1; l < 5 && k < 7; l++)
					{
						while (point[k] == point[j]) //�������Ƶ�����ͬ��������һ��
						{
							k++;
						}
						if (F && color[k] == color[j]);
						else
						{
							F = false;
						}
						if (point[j] - point[k]>1)	//����е������1������5�����޷�˳��
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
					//˳�Ӳ�ͬ���ķ���
					nowscore = 0;
					nowscore += 4 * pow(10.0, 10);
					nowscore += (point[i] * pow(10.0, 8));
					j = i;	//jΪ��һ����
					k = j + 1;	//kΪ��ǰ�ƣ����˳�ӣ����ܳ���k������j�������
					for (l = 1; l < 5 && k < 7; l++)
					{
						while (point[k] == point[j]) //�������Ƶ�����ͬ��������һ��
						{
							k++;
						}
						if (F && color[k] == point[j]);
						else
						{
							F = false;
						}
						if (point[j] - point[k]>1)	//����е������1������5�����޷�˳��
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
		//���ͬ��
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

	//������������ͬ���������ͷ���
	long long int checkOther()
	{
		long long int nowscore = 0;
		//���������ķ���
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
		//�����«�������ķ���
		else if (maxSum == 3)
		{
			for (int i = 14; i > 0; i--)
			{
				//��«�������
				if (pointSum[i] == 2)
				{
					nowscore += 6 * pow(10.0, 10);
					nowscore += (maxPoint*pow(10.0, 8));
					nowscore += i;
					return nowscore;
				}
			}
			//�����������
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
		//�������Ի�һ�Է���
		else if (maxSum == 2)
		{
			//���Է���

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
			//һ�Է���
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
		//���Ʒ���
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
	PairPoker APoker;	//ʵ���˿�
	PairPoker opPoker;	//ģ�����˿�
	PokerType Hold[2];	//�ҷ�����
	PokerType Pub[5];	//����
	PokerType opPub[5];	//ģ�⹫��
	int PubLen = 0;
	int opPubLen;			//ģ���ù�������
	struct node first[15][15][4][4];	//������Ƶ��������
	struct node *root;
	short int state;	//��5��ģ��׶Σ�δ�õ����ƣ��õ����ƣ�3�Ź��ƣ�4�ţ�5��
	int playerSum = 8;			//�������
	PokerType otherHold[8][2];	//�����˵���
	Cards myCards, enemyCards;	//�ҷ��������˷����Ƚ�
	long long int myscore, otherscore;
	int win = 0, visit = 0;//2,3,4�׶ε�ʤ�����
	//���ؿ���ģ��
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
	//�������
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
	//�ָ�ģ�����˿�
	void copyPoker()
	{
		memcpy(opPoker.All, APoker.All, sizeof(APoker.All));
		opPoker.len = APoker.len;
	}

	//�����ҷ�����
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

	//���������˷���
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