#include "StdAfx.h"
#include "Square.h"

/**
	Constructor.
*/
Square::Square(void)
{
	_x = 0;
	_y = 0;
	_color = WHITE;
	_empty = true;
	_piece = NULL;
}

/**
	Destructor.
*/
Square::~Square(void)
{
}

/**
	Renders the square.

	@author Christoffer Niska
	@return void
*/
void Square::render()
{
	if (_piece != NULL)
	{
		_piece->render();
	}
	else
	{
		std::cout << " ";
	}
}

/**
	Sets the color of the square. (0 = WHITE, 1 = BLACK)

	@author Christoffer Niska
	@param color the color
	@return void
*/
void Square::setColor(int color)
{
	_color = color;
}

/**
	Returns the color of the square. (0 = WHITE, 1 = BLACK)

	@author Christoffer Niska
	@return the color
*/
int Square::getColor()
{
	return _color;
}

/**
	Sets the given piece in the square.

	@author Christoffer Niska
	@param piece the piece
	@return void
*/
void Square::setPiece(Piece *piece)
{
	_piece = piece;
	_empty = piece == NULL;
}

/**
	Returns the piece in the square.

	@author Christoffer Niska
	@return the piece or NULL if no piece was found
*/
Piece *Square::getPiece()
{
	return _piece;
}

bool Square::isEmpty(){
	return _empty;
}

