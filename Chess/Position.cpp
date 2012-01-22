#include "StdAfx.h"
#include "Position.h"

/**
	Constructor.

	@author Christoffer Niska
 */
Position::Position(void)
{
	_board = new Board();

	// Castling is allowed until king or rook is moved.
	_whiteCastleShortAllowed = true;
	_whiteCastleLongAllowed = true;
	_blackCastleShortAllowed = true;
	_blackCastleLongAllowed = true;

	// Empty the board.
	_board->clear();

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
	_board->clear();
}

/**
	Sets the initial positions for the pieces on the board.

	@author Christoffer Niska
	@return void
 */
void Position::initPos()
{
	// Set the initial positions for the pieces on the board.
	_board->initPos();

	// White always starts the game.
	_toMove = WHITE;
}

/**
	Renders the board.

	@author Christoffer Niska, Mikko Malmari
	@return void
*/
void Position::render() const 
{
	_board->render();
}

/**
	Executes a move.

	@author Christoffer Niska, Mikko Malmari
	@param m the move
	@return void
*/
void Position::execMove(const Move *m)
{
	// Execute the move on the board.
	_board->execMove(m);

	// Turn over the turn.
	endTurn();
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

/**
	End the current turn turning over the turn to the other player.

	@author Christoffer Niska
	@return void
*/
void Position::endTurn()
{
	if (_toMove == WHITE)
	{
		_toMove = BLACK;
	}
	else
	{
		_toMove = WHITE;
	}
}

/**
	Returns the _toMove parameters std::string representative. 

	@author Olli Koskinen
	@return "Black" or "White"
*/
std::string Position::getTurn() const
{
	if (_toMove == BLACK)
	{
		return "Black";
	}
	else if (_toMove == WHITE)
	{
		return "White";
	}

	//Something went terribly, terribly wrong.
	return NULL;
}