#include "stdafx.h"

/**
	Main method.

	@author Christoffer Niska
	@param argc number of arguments
	@param argv argument values
	@return exit code
*/
int _tmain(int argc, _TCHAR* argv[])
{
	char *mv = new char[10];
	int turn = 0;
	int i, len, legalMoveCount;

	Board *board = new Board();
	Move *move = NULL;
	//Move *moveList = new Move[MAX_MOVES];

	while (true)
	{
		// Empty the screen.
		system("cls");

		if (move != NULL)
		{
			board->execMove(move);
		}

		// Render the board.
		board->print();

		std::cout << std::endl;
			
		if (move != NULL)
		{
			std::cout << " Last move: ";
			move->print();
		}

		std::cout << std::endl;

		/*
		legalMoveCount = board->genLegalMoves(moveList);

		for (i = 0; i < legalMoveCount; i++)
		{
			moveList[i].print();
		}
		*/

		std::cout << std::endl;
		std::cout << " Turn: " << board->getTurn()<<std::endl;
		std::cout << " Next move: ";
		std::cin >> mv;

		// Wait for a keypress.
		getchar();

		// Process the move.
		move = new Move();
		move->strToMove(mv);

		turn++;
	}

	return 0;
}

