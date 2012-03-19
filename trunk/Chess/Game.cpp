#include "StdAfx.h"

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

	bool input = true;
	char perf;
	int perfDepth;
	char white,black;

	//Promt for Perft test (The performance test for AI)
	std::cout<<"Perform Perft test (y/n): ";
	std::cin>>perf;

	if(perf == 'y'){
		std::cout<<"To what depth? (6 max): ";
		std::cin>>perfDepth;

		if(perfDepth >= 0 && perfDepth <= 6){
			for(int i = 0; i <= perfDepth; i++){
				std::cout<<"perft test with depth of "<<i <<" yielded "<<_board->Perft(i)<<"\n";
			}
			std::cout<<"\nStart of divide output: \n";
			//_board->divide(perfDepth);
		}else{
			std::cout<<"The depth must be from 0 to 6. Depth set to 1 by default."<<"\n";
			std::cout<<"perft test with depth of 1 yielded "<<_board->Perft(1)<<"\n";
		}
	}

	/*
	 *End of PERFT test
	 */

	while(input){
		std::cout<<"Choose AI player\n";
		std::cout<<"White is AI (y/n): ";
		std::cin>>white;
		std::cout<<"\nBlack is AI (y/n): ";
		std::cin>>black;

		if(white == 'y' || white=='n'){
			if(black == 'y' || black=='n'){
				input = false;
			}
		}

	}

	while (_running)
	{
		//Check if the move is legal and prompt the user if not
		while(!_board->moveIsLegal(_curMove))
		{

			_board->fiftyMoveRule();

			// Render the game.
			render();

			if(_toMove == WHITE){
				if(white == 'y'){ //AI plays
					_curMove = _board->wRootSearch();

				}else{ //the user plays
					std::cin >> moveStr;
					getchar();
					// Process the move.
					_curMove = new Move();
					_curMove->strToMove(moveStr);
				}
			}
			else{
				if(black == 'y'){//AI plays
					_curMove = _board->bRootSearch();

				}else{//the user plays
					std::cin >> moveStr;
					// Wait for a keypress.
					getchar();
					// Process the move.
					_curMove = new Move();
					_curMove->strToMove(moveStr);
				}
			}


			/*
			std::vector<std::string> muuvit = _board->getMoveStrings();
			if(muuvit.size() == 5)
			getchar();

			srand(time(0));

			if(debug && _toMove == BLACK){
			int rands= rand() % muuvit.size();
			std::string s = muuvit.at(rands);

			for(int i = 0; i <s.length(); i++){
			moveStr[i] = s.at(i);
			}
			}else {
			std::cin >> moveStr;
			// Wait for a keypress.
			getchar();
			// Process the move.

			}
			_curMove = new Move();
			_curMove->strToMove(moveStr);*/

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
		if(i == str.size() - 1)
				std::cout<<str.at(i);
		else
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

