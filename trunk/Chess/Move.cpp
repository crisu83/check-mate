#include "StdAfx.h"
#include "Move.h"

/**
	Constructor.

	@author Christoffer Niska
*/
Move::Move(void)
{
	_castleLong = false;
	_castleShort = false;
	_enPassant = false;
	_promoteTo = -1;
}

/**
	Constructor.

	@author Christoffer Niska
	@param x1 the source x-coordinate
	@param y1 the source y-coordinate
	@param x2 the destination x-coordinate
	@param y2 the destination y-coordinate
*/
Move::Move(int x1, int y1, int x2, int y2)
{
	Move();
	promote = false;

	_x1 = x1;
	_y1 = y1;
	_x2 = x2;
	_y2 = y2;
}

/**
	Destructor.
*/
Move::~Move(void)
{
}

/**
	Interprents a human-readable move string.

	@author Christoffer Niska, Mikko Malmari
	@param str the move as a string
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
		// todo: en passant

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
			_y1 = atoi(&c) - 1;
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
			_y2 = atoi(&c) - 1;

			//Promote 
			if(strlen(res) >=3){
				switch(res[2]){
					case 'n':
						promote = true;
						_promoteTo = 'n'; // use the ascii number as identifier
						break;			  // in this case 'n' would be equal to 110
					case 'b':
						promote = true;
						_promoteTo = 'b'; //This way we can compare the number in a form that is easy 
						break;			  //to read by the coder
					case 'q':
						promote = true;
						_promoteTo = 'q';
						break;
					case 'r':
						promote = true;
						_promoteTo = 'r';
						break;
				}
			}
		}

		res = strtok(NULL, "-");
		i++;
	}

	//castling flags for whites
	if((_x1 == 4 && _x2 == 2) && (_y1 == 0 && _y2 == 0)){
		_castleLong = true;
	}
	else if ((_x1== 4 && _x2 ==6) && (_y1 == 0 && _y2 == 0)){
		_castleShort = true;
	}	//castling flags for blacks
	else if((_x1 == 4 && _x2 == 2) && (_y1 == 7 && _y2 == 7)){
		_castleLong = true;
	}
	else if ((_x1== 4 && _x2 ==6) && (_y1 == 7 && _y2 == 7)){
		_castleShort = true;
	}
	
	// Free the memory.
	delete copy;
	delete res;
}

/**
	Prints the move as a human-readable string.

	@author Christoffer Niska, Mikko Malmari, Olli Koskinen
	@retrun void
*/
void Move::print() const
{
	// todo: handle castling, en passant and promotion.

	char str[10];

	if (!_castleLong && !_castleShort && !_enPassant && _promoteTo == -1)
	{

		str[0] = LETTERS[_x1];
		str[1] = '0' + (_y1 +1 ); // amazing logic! Modified by Olli, The +1 is because we subtracted 1 
		str[2] = '-';			  //from the y coord for better indexin with arrays
		str[3] = LETTERS[_x2];
		str[4] = '0' + (_y2 + 1); // amazing logic again!
		str[5] = '\0';
	}

	std::cout << str << std::endl; 
}

/**
	Returns the source x-coordinate.

	@return the coordinate
*/
int Move::getX1() const
{
	return _x1;
}

/**
	Returns the source y-coordinate.

	@return the coordinate
*/
int Move::getY1() const
{
	return _y1;
}

/**
	Returns the destination x-coordinate.

	@return the coordinate
*/
int Move::getX2() const
{
	return _x2;
}

/**
	Returns the destination y-coordinate.

	@return the coordinate
*/
int Move::getY2() const
{
	return _y2;
}

/**
	Returns true if castling is ongoing

	@return bool true if castling
*/
bool Move::Castling() const{
	return (_castleLong == true || _castleShort == true) ? true : false;
}

/**
	A function to check, if we can castle with particular rook
	@Author Olli Koskinen, Arttu Nieminen
	@return bool true if castling is possible
*/
bool Move::castlingLong() const{
	return _castleLong;
}

/**
	A function to check, if we can castle with particular rook
	@Author Olli Koskinen, Arttu Nieminen
	@return bool true if castling is possible
*/
bool Move::castlingShort() const{
	return _castleShort;
}


/**
	A function to return the piece we are going to promote to

	@Author Olli Koskinen, Arttu Nieminen
	@return int as the piece representative
*/
int Move::getPromoteTo() const{
	return _promoteTo;
}

/**
	A function to return boolean for checking if we can promote.

	@Author Olli Koskinen, Arttu Nieminen
	@return true if we can promote
*/
bool Move::promoting() const {
	return promote;
}

