// Chess.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char *mv = new char[10];;
	int turn = 0;
	Position *position = new Position();
	Move *move = NULL;

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
			move->print();
		}
		
		std::cout << std::endl << "Next move: ";
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
