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

	Position *position = new Position();
	Move *move = NULL;
	Move *moveList = new Move[MAX_MOVES];

	while (true)
	{
		// Empty the screen.
		system("cls");

		if (move != NULL)
		{
			position->execMove(move);
		}

		// Render the board.
		position->print();

		std::cout << std::endl;
			
		if (move != NULL)
		{
			std::cout << " Last move: ";
			move->print();
		}

		std::cout << std::endl;

		// Number of legal moves is currently always zero 
		// because it is yet to be implemented.
		legalMoveCount = position->genLegalMoves(moveList);

		for (i = 0; i < legalMoveCount; i++)
		{
			moveList[i].print();
		}

		std::cout << std::endl;
		
		std::cout << " Next move: ";
		std::cin >> mv;

		// Wait for a keypress.
		getchar();

		// Process the move.
		move = new Move();
		move->strToMove(mv);

		turn++;

		delete mv;
	}

	return 0;
}

