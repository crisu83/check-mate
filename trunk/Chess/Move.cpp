#include "StdAfx.h"
#include "Move.h"


Move::Move(void)
{
	_castleLong = false;
	_castleShort = false;
	_enPassant = false;
	_promoteTo = -1;
}


Move::~Move(void)
{
}

/**
	Interprents a human-readable move string.

	@author Christoffer Niska, Mikko Malmari
	@param str the move
	@return void
*/
void Move::strToMove(const char* str)
{
	char *copy = new char[strlen(str) + 1];
	char *res;
	char c;
	size_t i = 0, j;

	// Copy and tokenize the move string.
	strcpy(copy, str);
	res = strtok(copy, "-");

	while (res != NULL)
	{
		// todo: handle castling, en passant and promotion.

		// source
		if (i == 0)
		{
			// x-coord
			c = res[0];
			
			for (j = 0; j < 8; j++)
			{
				if (LETTERS[j] == c)
				{
					_x1 = j;
				}
			}

			// y-coord
			c = res[1];
			_y1 = 8 - atoi(&c);
		}
		// destination
		else
		{
			// x-coord
			c = res[0];

			for (j = 0; j < 8; j++)
			{
				if (LETTERS[j] == c)
				{
					_x2 = j;
				}
			}

			// y-coord
			c = res[1];
			_y2 = 8 - atoi(&c);
		}

		res = strtok(NULL, "-");
		i++;
	}
	
	// Free the memory.
	delete copy;
	delete res;
}


/**
	Prints the move as a human-readable string.

	@author Christoffer Niska, Mikko Malmari
	@retrun void
*/
void Move::print() const
{
	// todo: handle castling, en passant and promotion.

	char str[10];

	if (!_castleLong && !_castleShort && !_enPassant && _promoteTo == -1)
	{
		str[0] = LETTERS[_x1];
		str[1] = '0' + (8 - _y1); // amazing logic!
		str[2] = '-';
		str[3] = LETTERS[_x2];
		str[4] = '0' + (8 - _y2); // amazing logic again!
		str[5] = '\0';
	}

	std::cout << str << std::endl; 
}

int Move::getX1() const
{
	return _x1;
}

int Move::getY1() const
{
	return _y1;
}

int Move::getX2() const
{
	return _x2;
}

int Move::getY2() const
{
	return _y2;
}

