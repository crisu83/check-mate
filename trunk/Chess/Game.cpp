#include "StdAfx.h"
#include "Game.h"
#include <bitset>
#include <time.h>

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
	_board->setPosition(_position);
	_board->initPos();

	// White always starts the game.
	_toMove = WHITE;
	_position->setToMove(_toMove);
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
	_turnNum = 0;
	_running = true;

	while (_running)
	{
		//Check if the move is legal and prompt the user if not
		while(!_board->moveIsLegal(_curMove)){

			_board->fiftyMoveRule();

			// Render the game.
			render();

			std::vector<std::string> muuvit = _board->getMoveStrings();
			if(muuvit.size() == 5)
				//getchar();

				srand(time(0));

			if(debug){
				int rands= rand() % muuvit.size();
				std::string s = muuvit.at(rands);

				for(int i = 0; i <s.length(); i++){
					moveStr[i] = s.at(i);
				}
			}

			if(!debug){
				std::cin >> moveStr;
				// Wait for a keypress.
				getchar();
			}

			// Process the move.
			_curMove = new Move();
			_curMove->strToMove(moveStr);
		}

		delete _curMove;
		_curMove = NULL;

		endTurn();
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
	if(!debug)
		system("cls");

	// Render the board.
	_board->render();

	printAllPossibleMoves();

	if (_curMove != NULL && _turnNum != 0)
	{
		std::cout << "Last move: ";
		_curMove->print();
	}


	std::cout << "Turn "<<_turnNum<<" : " << getTurnName() << std::endl;

	std::cout << "Next move: ";
}

/**
	End the current turn turning over the turn to the other player.

	@author Christoffer Niska, Olli Koskinen
	@return void
*/
void Game::endTurn()
{
	_toMove = _toMove == WHITE ? BLACK : WHITE;
	_position->setToMove(_toMove);
}


/**
	Prints in human readable form all the possible moves we have generated

	@author Olli Koskinen, Arttu Nieminen
	@return void
*/
void Game::printAllPossibleMoves(){
	std::cout<<std::endl;
	std::vector<std::string> str = _board->getMoveStrings();
	int counter = 0;
	for(int i = 0; i < str.size(); i++){
		counter++;
		std::cout<<str.at(i)<<",";
		if(counter == 11)
			std::cout<<std::endl, counter = 0;
	}
	std::cout<<std::endl;
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

