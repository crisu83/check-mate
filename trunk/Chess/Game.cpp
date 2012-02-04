#include "StdAfx.h"
#include "Game.h"

/**
	Constructor.

	@author Christoffer Niska
*/
Game::Game(void)
{
	_board = NULL;
	_position = NULL;
	_curMove = NULL;

	init();
}

/**
	Destructor.
*/
Game::~Game(void)
{
}

/**
	Initializes the game.

	@author Christoffer Niska
	@return void
*/
void Game::init(void)
{
	_board = new Board();
	_position = new Position();
	_curMove = NULL;

	// Empty the position data.
	//_position->clear();
	//_position->initPos();

	// Empty the board.
	_board->clear();
	//_board->setPosition(_position);
	_board->initPos();

	// White always starts the game.
	_toMove = WHITE;
}

/**
	Runs the game.

	@author Christoffer Niska
	@return exit code
*/
int Game::run(void)
{
	char *moveStr = new char[10];
	Move *moveList = new Move[MAX_MOVES];

	_running = true;

	while (_running)
	{
		if (_curMove != NULL)
		{
			_board->execMove(_curMove);
			endTurn();
		}

		// Render the game.
		render();

		std::cin >> moveStr;

		// Wait for a keypress.
		getchar();

		// Process the move.
		_curMove = new Move();
		_curMove->strToMove(moveStr);

		_turnNum++;
	}

	delete moveList;
	delete moveStr;

	return 0;
}

/**
	Updates the game logic.

	@return void
*/
void Game::update(void)
{
	// todo: implement
}

/**
	Render a single frame of the game.

	@author Christoffer Niska
	@return void
*/
void Game::render(void)
{
	// Empty the screen.
	system("cls");

	// Render the board.
	_board->render();

	std::cout << std::endl;
			
	if (_curMove != NULL)
	{
		std::cout << " Last move: ";
		_curMove->print();
	}

	std::cout << std::endl;

	// todo: generate legal moves.

	std::cout << " Turn: " << getTurnName() << std::endl;

	std::cout << std::endl;

	std::cout << " Next move: ";
}

/**
	End the current turn turning over the turn to the other player.

	@author Christoffer Niska
	@return void
*/
void Game::endTurn()
{
	_toMove == WHITE ? BLACK : WHITE;
}

/**
	Returns the _toMove parameters std::string representative. 

	@author Olli Koskinen
	@author Christoffer Niska
	@return "Black" or "White"
*/
std::string Game::getTurnName() const
{
	return _toMove == WHITE ? "White" : "Black";
}

