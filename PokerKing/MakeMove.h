#ifndef MAKEMOVE_H
#define MAKEMOVE_H

#include "PokerType.h"
#include "MCTS.h"

enum MoveType
{
	BLIND, CHECK, CALL, RAISE, ALLIN, FOLD 
};

struct Move
{
	MoveType type;
	int param;
	Move(MoveType t, int x=0)
		:type(t)
		,param(x)
	{
	}
};

#define MAX_PLAYER 10
#define MAX_ID 10000

class MakeMove
{
private:

	MCTS* mcts;

	int myid;

	int totalBet;
	int playerBet[MAX_ID];
	int playerJetton[MAX_ID];

	int playerList[MAX_PLAYER];
	int playerNum;
	int mypos;

protected:

	int calcSupposedWager();

	Move makeDirectNextMove();
	Move makeCalcedNextMove();

public:

	MakeMove(MCTS& tree, int id);
	~MakeMove();

	void newPlayer(int id, int jetton, int money);
	void newCard(PokerType card, bool isHold);

	void makeEnemyMove(int id, int jetton, int money, int bet, MoveType type);
	void setTotalPot(int bet);

	Move makeNextMove();


};


#endif

