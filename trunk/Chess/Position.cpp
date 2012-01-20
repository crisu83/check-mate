#include "StdAfx.h"
#include "Position.h"

/**
	Constructor.

	@author Christoffer Niska
 */
Position::Position(void)
{
	// Empty the board.
	clear();

	// Initialize the piece positions.
	initPos();

	// Render the board.
	print();
}


Position::~Position(void)
{
}

/**
	Empties the board.

	@author Christoffer Niska
	@return void
 */
void Position::clear() 
{
	size_t x, y;

	for (x = 0; x < 8; x++)
	{
		for (y = 0; y < 8; y++)
		{
			_board[x][y] = 0;
		}
	}
}

/**
	Sets the initial positions for the pieces on the board.

	R N B Q K B N R - Black
	P P P P P P P P
	0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0
	P P P P P P P P
	R N B Q K B N R - White

	@author Christoffer Niska
	@return void
 */
void Position::initPos()
{
	size_t x;

	// Paws
	for (x = 0; x < 8; x++)
	{
		_board[1][x] = B_PAWN;
		_board[6][x] = W_PAWN;
	}

	// Rooks
	_board[0][0] = _board[0][7] = B_ROOK;
	_board[7][0] = _board[7][7] = W_ROOK;

	// Knights
	_board[0][1] = _board[0][6] = B_KNIGHT;
	_board[7][1] = _board[7][6] = W_KNIGHT;

	// Bishops
	_board[0][2] = _board[0][5] = B_BISHOP;
	_board[7][2] = _board[7][5] = W_BISHOP;

	// Queens
	_board[0][3] = B_QUEEN;
	_board[7][3] = W_QUEEN;

	// Kings
	_board[0][4] = B_KING;
	_board[7][4] = W_KING;
}

void Position::print() const 
{
	size_t x, y;

	for (x = 0; x < 8; x++)
	{
		for (y = 0; y < 8; y++)
		{
			char c;

			switch (_board[x][y])
			{
			case EMPTY:
				c = '#';
				break;

			case W_KING:
				c = 'K';
				break;

			case W_QUEEN:
				c = 'Q';
				break;

			case W_ROOK:
				c = 'R';
				break;

			case W_BISHOP:
				c = 'B';
				break;

			case W_KNIGHT:
				c = 'N';
				break;

			case W_PAWN:
				c = 'P';
				break;

			case B_KING:
				c = 'k';
				break;

			case B_QUEEN:
				c = 'q';
				break;

			case B_ROOK:
				c = 'r';
				break;

			case B_BISHOP:
				c = 'b';
				break;

			case B_KNIGHT:
				c = 'n';
				break;

			case B_PAWN:
				c = 'p';
				break;
			}

			std::cout << c;
		}

		std::cout << "\n";
	}

	getchar();
}