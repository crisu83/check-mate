#include "StdAfx.h"
#include "Position.h"

/**
	Constructor.

	@author Christoffer Niska
 */
Position::Position(void)
{
	// White always starts the game.
	_toMove = WHITE;

	// Castling is allowed until king or rook is moved.
	_whiteCastleShortAllowed = true;
	_whiteCastleLongAllowed = true;
	_blackCastleShortAllowed = true;
	_blackCastleLongAllowed = true;

	// Empty the board.
	clear();

	// Initialize the piece positions.
	initPos();
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
	int x, y;

	for (x = 0; x < 8; x++)
	{
		for (y = 0; y < 8; y++)
		{
			_board[x][y] = EMPTY;
		}
	}
}

/**
	Sets the initial positions for the pieces on the board.

	  a b c d e f g h
	8 R N B Q K B N R 8 - Black
	7 P P P P P P P P 7
	6 0 0 0 0 0 0 0 0 6
	5 0 0 0 0 0 0 0 0 5
	4 0 0 0 0 0 0 0 0 4
	3 0 0 0 0 0 0 0 0 3
	2 P P P P P P P P 2
	1 R N B Q K B N R 1 - White
	  a b c d e f g h 

	@author Christoffer Niska
	@return void
 */
void Position::initPos()
{
	int x;

	// Paws
	for (x = 0; x < 8; x++)
	{
		_board[x][1] = B_PAWN;
		_board[x][6] = W_PAWN;
	}

	// Rooks
	_board[0][0] = _board[7][0] = B_ROOK;
	_board[0][7] = _board[7][7] = W_ROOK;

	// Knights
	_board[1][0] = _board[6][0] = B_KNIGHT;
	_board[1][7] = _board[6][7] = W_KNIGHT;

	// Bishops
	_board[2][0] = _board[5][0] = B_BISHOP;
	_board[2][7] = _board[5][7] = W_BISHOP;

	// Queens
	_board[3][0] = B_QUEEN;
	_board[3][7] = W_QUEEN;

	// Kings
	_board[4][0] = B_KING;
	_board[4][7] = W_KING;
}

/**
	Renders the board.

	@author Christoffer Niska
	@return void
*/
void Position::print() const 
{
	int x, y;

	std::cout << std::endl;
	std::cout << "   a b c d e f g h " << std::endl;
	std::cout << "  +-+-+-+-+-+-+-+-+" << std::endl;

	for (y = 0; y < 8; y++)
	{
		std::cout << " " << 8 - y << "|";

		for (x = 0; x < 8; x++)
		{
			char c;

			switch (_board[x][y])
			{
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

			case EMPTY:
			default:
				c = ' ';
				break;
			}

			std::cout << c << "|";
		}

		std::cout << 8 - y << std::endl;
		std::cout << "  +-+-+-+-+-+-+-+-+" << std::endl;
	}

	std::cout << "   a b c d e f g h " << std::endl;
}

/**
	Executes a move.

	@param m the move
	@return void
*/
void Position::execMove(const Move *m)
{
	int x1, y1, x2, y2;
	int piece = EMPTY;

	x1 = m->getX1();
	y1 = m->getY1();
	x2 = m->getX2();
	y2 = m->getY2();

	piece = _board[x1][y1];

	if (piece != EMPTY)
	{
		_board[x2][y2] = piece;
		_board[x1][y1] = EMPTY;
	}
}