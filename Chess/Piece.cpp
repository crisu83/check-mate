#include "StdAfx.h"
#include "Piece.h"

/**
	Constructor.
 */
Piece::Piece(void)
{
}

/**
	Constructor.

	@author Christoffer Niska
	@param type the piece type
 */
Piece::Piece(int type)
{
	Piece();
	_type = -1;
	_type = type;
}

/**
	Destructor.
*/
Piece::~Piece()
{
}

/**
	Renders the piece

	@author Christoffer Niska
	@return void
*/
void Piece::render()
{
	char c;

	switch (_type)
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

	std::cout << c;
}

/**
	Returns the piece type.

	@return the type
*/
int Piece::getType()
{
	return _type;
}

