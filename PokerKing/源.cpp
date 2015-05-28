#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "MCTS.h"

int main()
{
	srand((unsigned)time(NULL));
	clock_t start, end;
	MCTS myTree;
	while (1)
	{
		for (int i = 0; i < 2; i++)
		{
			myTree.Hold[i].fromInput();
		}
		myTree.APoker.destroy(myTree.Hold[0]);
		myTree.APoker.destroy(myTree.Hold[1]);
		myTree.state = 1;

		myTree.Pub[0].fromInput();
		myTree.Pub[1].fromInput();
		myTree.Pub[2].fromInput();
		myTree.APoker.destroy(myTree.Pub[0]);
		myTree.APoker.destroy(myTree.Pub[1]);
		myTree.APoker.destroy(myTree.Pub[2]);		

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