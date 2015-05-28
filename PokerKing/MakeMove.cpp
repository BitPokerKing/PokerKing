#include "MakeMove.h"

#include <cstring>

MakeMove::MakeMove(MCTS& tree, int id)
	:mcts(&tree)
	,myid(id)
	,mypos(-1)
	,totalBet(0)
	,playerNum(0)
{
	memset(playerBet, 0, sizeof(playerBet));
	memset(playerJetton, 0, sizeof(playerJetton));
}

MakeMove::~MakeMove()
{

}

void MakeMove::newPlayer(int id, int jetton, int)
{
	playerList[playerNum]=id;
	playerJetton[id]=jetton;
	if(id==myid)
		mypos=playerNum;
	++playerNum;
}

void MakeMove::newCard(PokerType card, bool isHold)
{
	if(isHold)
	{
		// do something about hold cards
	}
	else
	{
		// do something about flop/turn/river cards
	}
}

void MakeMove::makeEnemyMove(int id, int jetton, int, int bet, MoveType)
{
	playerBet[id]=bet;
	playerJetton[id]=jetton;
}

void MakeMove::setTotalPot(int bet)
{
	totalBet=bet;
}

Move MakeMove::makeNextMove()
{
	return makeDirectNextMove();
}

int MakeMove::calcSupposedWager()
{
	return 0;
}

Move MakeMove::makeDirectNextMove()
{
	// do something with mcts
	return Move(FOLD);
}

Move MakeMove::makeCalcedNextMove()
{
	// do something else
	return Move(FOLD);
}

