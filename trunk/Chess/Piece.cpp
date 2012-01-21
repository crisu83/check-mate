#include "StdAfx.h"
#include "Piece.h"

Piece::Piece(void)
{
	_type = EMPTY;
	_x = 0;
	_y = 0;
}

/**
	Constructor.

	@author Christoffer Niska
	@param type the piece type
	@param x the x-coordinate
	@param y the y-coordinate
*/
Piece::Piece(int type, int x, int y)
{
	_type = type;
	_x = x;
	_y = y;
}

/**
	Destructor.
*/
Piece::~Piece(void)
{
}

/**
	Generates the legal moves for the piece.

	@author Christoffer Niska
*/
int Piece::genLegalMoves(Move *moveList)
{
	int moveCount = 0;

	// todo: implement

	return moveCount;
}

/**
	Returns the piece type.

	@return the type
*/
int Piece::getType() const
{
	return _type;
}