#include "StdAfx.h"
#include "Position.h"

/**
	Constructor.

	@author Christoffer Niska
 */
Position::Position(void)
{
	// Castling is allowed until king or rook is moved.
	_whiteCastleShortAllowed = true;
	_whiteCastleLongAllowed = true;
	_blackCastleShortAllowed = true;
	_blackCastleLongAllowed = true;

	// Initialize the positions.
	initPos();
}

/**
	Destructor.
*/
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
			_map[x][y] = EMPTY;
		}
	}
}

/**
	Sets the initial positions for the pieces on the board.

	@author Christoffer Niska
	@return void
 */
void Position::initPos()
{
	int x;

	// Paws
	for (x = 0; x < 8; x++)
	{
		_map[x][1] = B_PAWN;
		_map[x][6] = W_PAWN;
	}

	// Rooks
	_map[0][0] = B_ROOK;
	_map[7][0] = B_ROOK;
	_map[0][7] = W_ROOK;
	_map[7][7] = W_ROOK;

	// Knights
	_map[1][0] = B_KNIGHT;
	_map[6][0] = B_KNIGHT;
	_map[1][7] = W_KNIGHT;
	_map[6][7] = W_KNIGHT;

	// Bishops
	_map[2][0] = B_BISHOP;
	_map[5][0] = B_BISHOP;
	_map[2][7] = W_BISHOP;
	_map[5][7] = W_BISHOP;

	// Queens
	_map[3][0] = B_QUEEN;
	_map[3][7] = W_QUEEN;

	// Kings
	_map[4][0] = B_KING;
	_map[4][7] = W_KING;
}

/**
	Generates all the legal moves.
	
	@author Christoffer Niska
	@param moveList the list in which to store the moves
	@return the number of legal moves
*/
int Position::genLegalMoves(Move *moveList)
{
	int moveCount = 0;


	return moveCount;
}

int *Position::getMap()
{
	return *_map;
}