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
	char white,black;

	if(DEBUG)
		debugPerfPrint();

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
		//Check if the move is legal and prompt the user if not otherwise move
		while(!_board->moveIsLegal(_curMove))
		{
			//Check the fifty move rule
			_board->fiftyMoveRule();

			// Render the game.
			render();

			if(_toMove == WHITE){
				if(white == 'y')
				{ //AI plays
					_curMove = _board->wRootSearch();

				}
				else//the user plays
				{ 

					std::cin >> moveStr;
					
					if(*moveStr == '0')
						debugBitboardPrinter();

					// Process the move.
					_curMove = new Move();
					_curMove->strToMove(moveStr);
				}
			}
			else //Black moves
			{
				if(black == 'y'){//AI plays
					_curMove = _board->bRootSearch();

				}
				else
				{//the user plays

					std::cin >> moveStr;

					if(*moveStr == '0')
						debugBitboardPrinter();

					// Process the move.
					_curMove = new Move();
					_curMove->strToMove(moveStr);
				}
			}
		}

		delete _curMove;
		_curMove = NULL;

		endTurn();
		_turnNum++;
	}

	delete[] moveList;
	delete[] moveStr;

	return 0;
}


/**
	Promt for Perft test (The performance test for AI)
	for debugging purposes

	@author Olli Koskinen, Arttu Nieminen
	@return void
*/
void Game::debugPerfPrint(){
	//We get some timings from the PERFT. in Milliseconds.
	ChessTimer ct;
	ct.changeTimerResolution(MILLISECONDS);

	char perf;
	int perfDepth;
	std::cout<<"Perform Perft test (y/n): ";
	std::cin>>perf;

	if(perf == 'y'){
		std::cout<<"To what depth? (6 max): ";
		std::cin>>perfDepth;

		if(perfDepth >= 0 && perfDepth <= 6){
			for(int i = 1; i <= perfDepth; i++){
				ct.StartCounter();
				std::cout<<"perft test with depth of "<<i <<" yielded "<<_board->Perft(i)<<"\n";
				_board->PerftResults();

				std::cout<<ct.GetCounter()<<" ms\n";


				//_board->BitBoardToMoves();
				//_board->render();
			}

			std::cout<<ct.getAverage();
		}else{
			std::cout<<"The depth must be from 0 to 6. Depth set to 1 by default."<<"\n";
			std::cout<<"perft test with depth of 1 yielded "<<_board->Perft(1)<<"\n";
		}
	}

	/*
	*End of PERFT test
	*/
}



/**
  Performs the printing of different bitboards
  for debugging purposes.

  @author Olli Koskinen, Arttu Nieminen
  @return void
*/
void Game::debugBitboardPrinter(){
	int perfDepth = -1;
	while(perfDepth != 0){
		std::cout<<"Divided (1-6)\nPrinting Bitboards 10: empty, 11:bpiec,12:wpiec,13:bq,14:wQ,"
			<<"\n 15:bk,16:wK,17:bRoo,18:wRoo,19:bbish,20:wbish,21:bknig,22:wknig,23:bpawn,24:wpawn\n"
			<< " 25:wAttack, 26:bAttack: ";
		std::cin>>perfDepth;

		while ( !( std::cin>>perfDepth ) ) {
			std::cerr<<"Invalid input detected\n";
			// Clean up the mess by discarding the entire line
			std::cin.clear();
			std::cin.ignore ( sizeof(long long) , '\n' );
		}


		if(perfDepth >= 0 && perfDepth <= 50){
			switch(perfDepth){
			case 0:
				perfDepth = 0;
				break;
			default:{
				std::cout<<"\nStart of divide output: \n";
				_board->divided(perfDepth);
				std::cout<<"tomove: "<<_position->getToMove();
				break;
					}
			case 10: {
				std::cout<<"Emptysquares bitboard:\n";
				_board->superHiddenRenderEmptySquares(15);
				break;}
			case 11:{
				std::cout<<"B_PIECES bitboard:\n";
				_board->superHiddenRenderEmptySquares(14);
				break;}
			case 12:{
				std::cout<<"W_PIECES bitboard:\n";
				_board->superHiddenRenderEmptySquares(13);
				break;}
			case 13:{
				std::cout<<"B_QUEEN bitboard:\n";
				_board->superHiddenRenderEmptySquares(8);
				break;}
			case 14:{
				std::cout<<"W_QUEEN bitboard:\n";
				_board->superHiddenRenderEmptySquares(2);
				break;}
			case 15:{
				std::cout<<"B_KING bitboard:\n";
				_board->superHiddenRenderEmptySquares(7);
				break;
					}
			case 16:{
				std::cout<<"W_KING bitboard:\n";
				_board->superHiddenRenderEmptySquares(1);
				break;
					}
			case 17:{
				std::cout<<"B_ROOK bitboard:\n";
				_board->superHiddenRenderEmptySquares(9);
				break;
					}
			case 18:{
				std::cout<<"W_ROOK bitboard:\n";
				_board->superHiddenRenderEmptySquares(3);
				break;
					}
			case 19:{
				std::cout<<"B_BISHOP bitboard:\n";
				_board->superHiddenRenderEmptySquares(10);
				break;
					}
			case 20:{
				std::cout<<"W_BISHOP bitboard:\n";
				_board->superHiddenRenderEmptySquares(4);
				break;
					}
			case 21:{
				std::cout<<"B_KNIGHT bitboard:\n";
				_board->superHiddenRenderEmptySquares(11);
				break;
					}
			case 22:{
				std::cout<<"W_KNIGHT bitboard:\n";
				_board->superHiddenRenderEmptySquares(5);
				break;
					}
			case 23:{
				std::cout<<"B_PAWN bitboard:\n";
				_board->superHiddenRenderEmptySquares(12);
				break;
					}
			case 24:{
				std::cout<<"W_PAWN bitboard:\n";
				_board->superHiddenRenderEmptySquares(6);
				break;
					}
			case 25:{
				std::cout<<"wAttack bitboard:\n";
				_board->superHiddenRenderEmptySquares(30);
				break;}
			case 26:{
				std::cout<<"bAttack bitboard:\n";
				_board->superHiddenRenderEmptySquares(40);
				break;}
			case 27:{
				std::cout<<"En Passant bitboard:\n";
				_board->superHiddenRenderEmptySquares(16);
				break;}
			}
		}
	}
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
	if(!DEBUG)
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

