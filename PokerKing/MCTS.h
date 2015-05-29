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

class Cards : public CardsBase
{
public:
	bool existA;			//�Ƿ����Ace
	long long int score = 0, Maxs;

	//���㵱ǰ7������5���������Ƶķ���
	void calMaxScore();

private:
	short int point[7];			//7���Ƶĵ���
	short int color[7];			//7���ƵĻ�ɫ
	short int colorSum[4];		//����ɫ������
	short int pointSum[15];		//������������
	short int maxSum = 0;		//��¼һ�Ի�����������
	short int maxPoint = 0;		//��¼����������Ƶ�������������Ƶĵ���

	//����Ƿ����Ace
	void checkAexist();

	//��7���Ƶ���תΪInt���Ҽ�¼��ɫ���͵�������
	void calpoint(int Atype);

	//��7���Ƶ����Ӵ�С����
	void sortpoint();

	//����Ƿ�Ϊͬ��˳�Լ�ͬ����˳��
	long long int checkSF();

	//������������ͬ���������ͷ���
	long long int checkOther();
};

class MCTS
{
public:
	PairPoker APoker;	//ʵ���˿�
	PairPoker opPoker;	//ģ�����˿�
	PokerType Hold[2];	//�ҷ�����
	PokerType opHold[2];//ģ��ʱ�ҷ�����
	PokerType Pub[5];	//����
	PokerType opPub[5];	//ģ�⹫��
	int HoldLen = 0, opHoldLen;
	int PubLen = 0;
	int opPubLen;			//ģ���ù�������
	node first[15][15][4][4];	//������Ƶ��������
	node *root;
	short int state;	//��5��ģ��׶Σ�δ�õ����ƣ��õ����ƣ�3�Ź��ƣ�4�ţ�5��
	int playerSum = 8;			//�������
	PokerType otherHold[8][2];	//�����˵���
	Cards myCards, enemyCards;	//�ҷ��������˷����Ƚ�
	long long int myscore, otherscore;
	int win = 0, visit = 0;//2,3,4�׶ε�ʤ�����

	mutex mtx; //����Э����չ�̺߳ͻ�ȡ������̵Ļ�����
	thread* threadExpand; //������չ�߳�
	bool threadEnded; //�����չ�Ƿ����

	//���ؿ���ģ��
	void MonteCarlo(int t=10000);

	//���õ���
	void setHold(PokerType pok);

	//���ù���
	void setPub(PokerType pok);

	//��ʼ������
	void initPok();

	//��ȡ��ǰʤ��
	double getRate();

	//���ʤ��
	void resetRate();

	//��state==0��1ʱ�����ʤ��
	void tryResetRate();

private:

	//�߳�������
	static void threadMC(MCTS* inst);

	//��ʼ�߳�
	void beginThread();

	//�����߳�
	void endThread();

	//�������
	void sortHold();

	//�ָ�ģ�����˿�
	void copyPoker();

	//�����ҷ�����
	void calMyScore();

	//���������˷���
	void calOtherScore(int n);

	
};
#endif
