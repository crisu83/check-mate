#include "stdafx.h"


/**
	Constructor.
 */
Board::Board(void)
{
	chessTimer = ChessTimer();
	initBitboards();
}

/**
	Destructor.

	@author Christoffer Niska
*/
Board::~Board(void)
{
	// todo: free mem.
}

/**
	Empties the board.

	@author Christoffer Niska
	@return void
 */
void Board::clear() 
{
	int x, y;

	for (x = 0; x < 8; x++)
	{
		for (y = 0; y < 8; y++)
		{
			_squares[x][y] = new Square();
		}
	}
}


/**
	Initializes the bitboards with starting positions for every piece
	for example: 

	White Pawns
	0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0		=> 0xFF00 in hex
	0 0 0 0 0 0 0 0
	0 0 0 0 0 0 0 0
	1 1 1 1 1 1 1 1
	0 0 0 0 0 0 0 0


	@author Olli Koskinen, Arttu Nieminen	
	@return void
 */
void Board::initBitboards(){

	fillSquareBits();
	fiftyMove = 0;


	_BitBoards[ EMPTY	]	= 0x0000;  // Empty board
	_BitBoards[ W_KING	]	= 0x10;
	_BitBoards[ W_QUEEN ]	= 0x8;
	_BitBoards[ W_ROOK	]	= 0x81;
	_BitBoards[ W_BISHOP]	= 0x24;
	_BitBoards[ W_KNIGHT]	= 0x42;
	_BitBoards[ W_PAWN	]	= 0xff00;
	_BitBoards[ B_KING	]	= 0x1000000000000000;
	_BitBoards[ B_QUEEN ]	= 0x800000000000000;
	_BitBoards[ B_ROOK	]	= 0x8100000000000000;
	_BitBoards[ B_BISHOP]	= 0x2400000000000000;
	_BitBoards[ B_KNIGHT]	= 0x4200000000000000;
	_BitBoards[ B_PAWN	]	= 0xff000000000000;
	_BitBoards[ W_PIECES ]  = _BitBoards[ W_PAWN ] | _BitBoards[ W_ROOK ]  |_BitBoards[ W_KNIGHT ] |_BitBoards[ W_BISHOP ]  | _BitBoards[ W_QUEEN ]  | _BitBoards[ W_KING ];
	_BitBoards[ B_PIECES ]  = _BitBoards[ B_PAWN ] | _BitBoards[ B_ROOK ]  |_BitBoards[ B_KNIGHT ] |_BitBoards[ B_BISHOP ]  | _BitBoards[ B_QUEEN ]  | _BitBoards[ B_KING ];
	_BitBoards[EMPTYSQUARES]= ~(_BitBoards[ W_PIECES ] | _BitBoards[ B_PIECES ]);
	_BitBoards[ ENPASSANT ] = 0x0000;
}


UI64 Board::getBitBoard( int type ){
	return _BitBoards[ type ];
}



/**
	Sets the initial positions for the pieces on the board.

	    a   b   c   d   e   f   g   h
	  +---+---+---+---+---+---+---+---+
	8 | r | n | b | q | k | b | n | r | 8 - Black
	  +---+---+---+---+---+---+---+---+
	7 | p | p | p | p | p | p | p | p | 7
	  +---+---+---+---+---+---+---+---+
	6 |   |   |   |   |   |   |   |   | 6
	  +---+---+---+---+---+---+---+---+
	5 |   |   |   |   |   |   |   |   | 5
	  +---+---+---+---+---+---+---+---+
	4 |   |   |   |   |   |   |   |   | 4
	  +---+---+---+---+---+---+---+---+
	3 |   |   |   |   |   |   |   |   | 3
	  +---+---+---+---+---+---+---+---+
	2 | P | P | P | P | P | P | P | P | 2
	  +---+---+---+---+---+---+---+---+
	1 | R | N | B | Q | K | B | N | R | 1 - White
	  +---+---+---+---+---+---+---+---+
	    a   b   c   d   e   f   g   h

	@author Christoffer Niska, Mikko Malmari
	@return void
 */
void Board::initPos()
{
	/*
	if (_position != NULL)
	{		
		
	}
	*/

	int x;

	// Pawns
	for (x = 0; x < 8; x++)
	{
		setPieceAt(x, 1, new Piece(W_PAWN));
		setPieceAt(x, 6, new Piece(B_PAWN));
	}

	// Rooks
	setPieceAt(0, 0, new Piece(W_ROOK));
	setPieceAt(7, 0, new Piece(W_ROOK));
	setPieceAt(0, 7, new Piece(B_ROOK));
	setPieceAt(7, 7, new Piece(B_ROOK));

	// Knights
	setPieceAt(1, 0, new Piece(W_KNIGHT));
	setPieceAt(6, 0, new Piece(W_KNIGHT));
	setPieceAt(1, 7, new Piece(B_KNIGHT));
	setPieceAt(6, 7, new Piece(B_KNIGHT));

	// Bishops
	setPieceAt(2, 0, new Piece(W_BISHOP));
	setPieceAt(5, 0, new Piece(W_BISHOP));
	setPieceAt(2, 7, new Piece(B_BISHOP));
	setPieceAt(5, 7, new Piece(B_BISHOP));

	// Queens
	setPieceAt(3, 0, new Piece(W_QUEEN));
	setPieceAt(3, 7, new Piece(B_QUEEN));

	// Kings
	setPieceAt(4, 0, new Piece(W_KING));
	setPieceAt(4, 7, new Piece(B_KING));
}

/**
	Renders the board.

	@author Christoffer Niska, Mikko Malmari
	@return void
*/
void Board::render()
{
	int x, y;
	Square *square;

	std::cout << std::endl;
	std::cout << "      a   b   c   d   e   f   g   h" << std::endl;
	std::cout << "    +---+---+---+---+---+---+---+---+" << std::endl;

	for (y = 0; y < 8; y++)
	{
		std::cout << "  " << 8 - y << " | ";

		for (x = 0; x < 8; x++)
		{
			square = _squares[x][7-y];

			// We do not care about the case that no square was found
			// because it is highly unlikely that it will happen.
			if (square != NULL)
			{
				square->render();
			}

			std::cout << " | ";
		}

		std::cout << 8 - y << std::endl;
		std::cout << "    +---+---+---+---+---+---+---+---+" << std::endl;
	}

	std::cout << "      a   b   c   d   e   f   g   h" << std::endl;

	// todo: free mem.
}

/**
	Executes a move on the board.

	@author Christoffer Niska, Mikko Malmari
	@param m the move
	@return void
*/
void Board::execMove(const Move *m)
{
	int x1, y1;
	int x2, y2;
	Piece *piece;

	// Source coordinates
	x1 = m->getX1();
	y1 = m->getY1();
	
	// Destination coordinates
	x2 = m->getX2();
	y2 = m->getY2();

	piece = getPieceAt(x1, y1);

	if (piece != NULL)
	{
		// Manipulate the board.
		setPieceAt(x2, y2, piece);
		setPieceAt(x1, y1, NULL);
	}
	// todo: free mem.
}


/**
	Transforms bitboards to Move objects

	@author Olli Koskinen, Arttu Nieminen
*/
void Board::BitBoardToMoves(){
	clear();
	//All the bitboards for pieces
	for(int i = 1; i <= B_PAWN; i++){
		for(int j = 0; j < SQUARES; j++){
			if((_BitBoards[i] & _SquareBits[ j ]) ==  _SquareBits[ j ] ){
				int x =  j & 7;
				int y =  j >> 3; 
				setPieceAt(x,y, new Piece(i));
			}
		}
	}
}

/**
	Transforms bitboards to Move objects

	@author Olli Koskinen, Arttu Nieminen
*/
Move *Board::BitBoardToMoves(std::vector<UI64> move){
	//All the bitboards for pieces
	int x1,y1,x2,y2;
	UI64 a = move.at(0);
	UI64 b = move.at(1);
	//find the index
	int j = bitScanForward(a);
	//Convert it to x&y
	x1 = j & 7;
	y1 = j >> 3;

	j = bitScanForward(b);
	x2 = j & 7;
	y2 = j >> 3;
	
	return new Move(x1,y1,x2,y2);
}


/**
 * bitScanForward
 * @author Matt Taylor
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
int Board::bitScanForward(UI64 bb) {
	const int lsb_64_table[64] =
{
   63, 30,  3, 32, 59, 14, 11, 33,
   60, 24, 50,  9, 55, 19, 21, 34,
   61, 29,  2, 53, 51, 23, 41, 18,
   56, 28,  1, 43, 46, 27,  0, 35,
   62, 31, 58,  4,  5, 49, 54,  6,
   15, 52, 12, 40,  7, 42, 45, 16,
   25, 57, 48, 13, 10, 39,  8, 44,
   20, 47, 38, 22, 17, 37, 36, 26
};
 
   unsigned int folded;
   assert (bb != 0);
   bb ^= bb - 1;
   folded = (int) bb ^ (bb >> 32);
   return lsb_64_table[folded * 0x78291ACF >> 26];
}



/**
	Updates the move to bitboards

	@author Olli Koskinen, Arttu Nieminen
	@return void
*/
void Board::updateBitBoards(Move move, int type){

	//Find the index for the squareBits array from the coords.
	int sourceIndex =	(move.getX1()) + (move.getY1()<<3);
	int destIndex	=	(move.getX2()) + (move.getY2()<<3);
	int toMove = _position->getToMove();

	//If the type is one of the Whites, then we look from black pieces
	//And if the type is one of the blacks, then we look from white pieces
	int enemyPieces = toMove == WHITE ? B_PIECES : W_PIECES;
	int ourPieces	= toMove == WHITE ? W_PIECES : B_PIECES;


	/**
	 *
	 *Fifty move rule
	 *
	 */
	if(type == W_PAWN ||type ==  B_PAWN)
		fiftyMove = 0;
	else
		fiftyMove++;


	/*
	*
	*	EN PASSANT
	*
	*/
	if(type == W_PAWN){
		if( (move.getY2() - move.getY1()) == 2){ //If we do a doublepush with pawn
			_BitBoards[ ENPASSANT ] |= _SquareBits[ sourceIndex ] << 8; 
		}

	}else if (type == B_PAWN){
		if( (move.getY1() - move.getY2()) == 2){ //If we do a doublepush with pawn
			_BitBoards[ ENPASSANT ] |= _SquareBits[ sourceIndex ] >> 8; 
		}
	}



	/*
	*
	*	CASTLING
	*
	*/

	//If we move the king, we cant do castling anymore
	if(type == W_KING){
		_position->setWhiteCastlingFalse();
	}
	else if(type == B_KING){
		_position->setBlackCastlingFalse();
	}


	//If we move one of the rooks, then it will not be able to castle anymore
	if(type == W_ROOK){
		//LS1B
		if((_BitBoards[W_ROOK] & -_BitBoards[W_ROOK]) & _SquareBits[0] ) //White queen side rook
			_position->wLongCastleFalse();
		//Reseted LS1B
		if( !( (_BitBoards[W_ROOK] ^(_BitBoards[W_ROOK] & -_BitBoards[W_ROOK])) & _SquareBits[7])) //White king side rook
			_position->wShortCastleFalse();
	}
	else if(type == B_ROOK){
		if((_BitBoards[B_ROOK] & -_BitBoards[B_ROOK]) & _SquareBits[63] ) //Black queen side rook
			_position->bShortCastleFalse();
		if(  (_BitBoards[B_ROOK] ^(_BitBoards[B_ROOK] & -_BitBoards[B_ROOK])) & _SquareBits[56])  //Black king side rook
			_position->bShortCastleFalse();
	}

	//If we have a castling situation
	if(move.Castling()){
		//We determine which rooks should we move with castling
		int rooksToMove	= ( type <= W_PAWN && type > EMPTY ) ? W_ROOK : B_ROOK;

		//Delete the king we are going to move and then add it to a new place
		_BitBoards[ type ] &=  ~_SquareBits[ sourceIndex ];
		_BitBoards[ type ] |=   _SquareBits[ destIndex   ];


		//We move rooks too
		if(move.castlingLong()){
			UI64 rooksPlace = (_BitBoards[ rooksToMove ] & -_BitBoards[ rooksToMove ]);
			//Delete the piece we are going to move and then add it to a new place
			_BitBoards[ rooksToMove ] &=  ~(_BitBoards[ rooksToMove ] & -_BitBoards[ rooksToMove ]);
			_BitBoards[ rooksToMove ] |=   rooksPlace << 3;

		}
		else if(move.castlingShort()) {
			//Delete the piece we are going to move and then add it to a new place
			UI64 rooksPlace =(_BitBoards[ rooksToMove ] ^(_BitBoards[ rooksToMove ] & -_BitBoards[ rooksToMove ]));
			_BitBoards[ rooksToMove ] &=  ~ (_BitBoards[ rooksToMove ] ^(_BitBoards[ rooksToMove ] & -_BitBoards[ rooksToMove ]));
			_BitBoards[ rooksToMove ] |=   rooksPlace  >> 2 ;
		}
	}


	/*
	*
	*	PROMOTING
	*
	*/
	else if(move.promoting() && toMove == WHITE && move.getY2() == 7 ){

		switch(move.getPromoteTo()){

		case 'n':
			//Knights
			//Remove the pawn from the list
			_BitBoards[ W_PAWN ] &=  ~_SquareBits[ sourceIndex ];
			//And ad a knight to the dest place
			_BitBoards[ W_KNIGHT] |=   _SquareBits[ destIndex   ];
			break;

		case 'b':
			//Bishops
			//Remove the pawn from the list
			_BitBoards[ W_PAWN ] &=  ~_SquareBits[ sourceIndex ];
			//And ad a bishop to the dest place
			_BitBoards[ W_BISHOP ] |=   _SquareBits[ destIndex   ];
			break;		

		case 'q':
			//Queens
			//Remove the pawn from the list
			_BitBoards[ W_PAWN ] &=  ~_SquareBits[ sourceIndex ];
			//And ad a queen to the dest place
			_BitBoards[ W_QUEEN] |=   _SquareBits[ destIndex   ];
			break;


		case 'r':
			//Rooks
			//Remove the pawn from the list
			_BitBoards[ W_PAWN ] &=  ~_SquareBits[ sourceIndex ];
			//And ad a rook to the dest place
			_BitBoards[ W_ROOK] |=   _SquareBits[ destIndex   ];
			break;

		}

	}
	else if (move.promoting() && toMove == BLACK && move.getY2() == 0){

		switch(move.getPromoteTo()){
		case 'n':
			//Knights
			//Remove the pawn from the list
			_BitBoards[ B_PAWN ] &=  ~_SquareBits[ sourceIndex ];
			//And ad a knight to the dest place
			_BitBoards[ B_KNIGHT] |=   _SquareBits[ destIndex   ];
			break;	

		case 'b':
			//Bishops
			//Remove the pawn from the list
			_BitBoards[ B_PAWN ] &=  ~_SquareBits[ sourceIndex ];
			//And ad a Bishop to the dest place
			_BitBoards[ B_BISHOP] |=   _SquareBits[ destIndex   ];
			break;		

		case 'q':
			//Queens
			//Remove the pawn from the list
			_BitBoards[ B_PAWN ] &=  ~_SquareBits[ sourceIndex ];
			//And ad a B_QUEEN to the dest place
			_BitBoards[ B_QUEEN] |=   _SquareBits[ destIndex   ];
			break;

		case 'r':
			//Rooks
			//Remove the pawn from the list
			_BitBoards[ B_PAWN ] &=  ~_SquareBits[ sourceIndex ];
			//And ad a B_ROOK to the dest place
			_BitBoards[ B_ROOK] |=   _SquareBits[ destIndex   ];
			break;

		}
	}


	/*
	*
	*	ATTACKS
	*
	*/
	//We look if there is enemy on our destination
	if( (_BitBoards[ enemyPieces ]  &  _SquareBits[ destIndex ]) == _SquareBits[ destIndex ] ){

		//First, clear the fifty move rule to zero, since we attack
		fiftyMove = 0;

		_BitBoards[ enemyPieces ]  &=  ~_SquareBits[  destIndex  ];  // Remove the enemy piece from pieces
		_BitBoards[  ourPieces	]  &=  ~_SquareBits[ sourceIndex ];	 // remove our piece from pieces

		//If we are promoting, these are not needed
		if(!move.promoting()){
			_BitBoards[	   type		]  &=  ~_SquareBits[ sourceIndex ];  // Remove our piece from the source
			_BitBoards[    type     ]  |=   _SquareBits[  destIndex  ];  // add our piece to destination
		}

		int i	= toMove == WHITE ? B_KING : W_KING;
		int end = toMove == WHITE ? B_PAWN : W_PAWN;
		//And just in case we search every enemy table trhough if there's still someone in our dest square
		for( i; i <= end; i++){
			if(( _BitBoards[ i ] & _SquareBits[ destIndex ]) == _SquareBits[destIndex ] ){
				_BitBoards[ i ]  &=  ~_SquareBits[destIndex ];
			}
		}
	}
	else{
		//Delete the piece we are going to move and then add it to a new place
		if(!move.promoting()){
			//If we have enpassant move
			if((_BitBoards[ ENPASSANT ]  & _SquareBits[destIndex]) == _SquareBits[destIndex] ){
				if(toMove == WHITE)
					_BitBoards[ B_PAWN ] &=  ~_SquareBits[ destIndex ] >> 8;
				else
					_BitBoards[ W_PAWN ] &=  ~_SquareBits[ destIndex ] << 8;
			}
			_BitBoards[ type ] &=  ~_SquareBits[ sourceIndex ];
			_BitBoards[ type ] |=   _SquareBits[ destIndex   ];
		}
	}


	//Update the all the white/black pieces according to turn
	_BitBoards[ W_PIECES ]  = _BitBoards[ W_PAWN ] | _BitBoards[ W_ROOK ] 
	| _BitBoards[ W_KNIGHT ] | _BitBoards[ W_BISHOP ]  | _BitBoards[ W_QUEEN ]  | _BitBoards[ W_KING ];

	_BitBoards[ B_PIECES ]  = _BitBoards[ B_PAWN ] | _BitBoards[ B_ROOK ] 
	| _BitBoards[ B_KNIGHT ] | _BitBoards[ B_BISHOP ]  | _BitBoards[ B_QUEEN ]  | _BitBoards[ B_KING ];


	_BitBoards[ EMPTYSQUARES ]= ~( _BitBoards[ W_PIECES ] | _BitBoards[ B_PIECES ] );



	if(debug)
		std::cout<<"Bitboards [ type ] = "<<_BitBoards[ type ]<<"\n"; 

	BitBoardToMoves();

	_BitBoards[ ENPASSANT ] &= toMove == WHITE ? ~SIXTH_RANK : ~THIRD_RANK;
}

/**
	Fills the SquareBits array with bits that represents the squares

	@author Olli Koskinen, Arttu Nieminen
	@return void
*/

void Board::fillSquareBits(){

	for(int i = 0; i< SQUARES; i++){
		_SquareBits[i] = 1ULL << i;
	}

}

/**
	Converts the x- and y-coordinates to a SquareBit representation

	@author Olli Koskinen, Arttu Nieminen
	@param x the x-coordinate
	@param y the y-coordinate
	@return UI64 SquareBit representation
*/
UI64 Board::posToSquare(int x, int y){
	return _SquareBits[8*y+x];
}


/**
	Checks if the move made by user is legal. 
	Changes the move to bitboard presentation and then ANDs it against the legal bitboards

	@author Olli Koskinen, Arttu Nieminen
	@param square the bit representation of move
	@return true if the move is legal
*/
bool Board::moveIsLegal(Move *_curMove){
	
	if(_curMove == NULL){
		return false;
	}

	if(getPieceAt(_curMove->getX1(),_curMove->getY1())-> getType() == 0){
		return false;
	}

	int sourceIndex =	(_curMove->getX1()) + (_curMove->getY1()<<3);
	int destIndex	=	(_curMove->getX2()) + (_curMove->getY2()<<3); 

	/**
	*
	* genLegalMoves and it's timing functions, for performance testing.
	*/

	chessTimer.StartCounter();
	std::vector<std::vector<UI64>> move = _position->genLegalMoves(_BitBoards);

	std::cout <<"\ngenLegalMoves inclusive time: "<<chessTimer.GetCounter()<<" microseconds\n";

	/**
	*
	* END
	*/


	for(int i = 0; i <move.size(); i++){
		UI64 source		  = move.at(i).at(0);
		UI64 destinations = move.at(i).at(1);

		if( (source & _SquareBits[sourceIndex]) != 0 ){
			if( (destinations &  _SquareBits[ destIndex ]) != 0 ){
				updateBitBoards(*_curMove, getPieceAt(_curMove->getX1(),_curMove->getY1())-> getType());
				std::cout <<"\nboard evaluation: "<< _position->evaluate(_BitBoards);
				return true;
			}
		}
	}

	return false;
}

/**
	Checks if we have moved 50 without captures or moving with pawn.  Game will draw after this.
	
	@Author Olli Koskinen, Arttu Nieminen
	@return true if we have moved silently 50 or more times.

*/
void Board::fiftyMoveRule(){
	if(fiftyMove >= 50 ){
		PlaySound(L"gameover.wav",NULL,SND_FILENAME|SND_ASYNC); 
		std::cout<<"Stalemate by 50 move rule!\nThe game ends in draw!";
		std::cout<<"\ngenLegalMoves average time was: "<<chessTimer.getAverage()<<" microseconds";
		std::cout<<"\nTimes called genLegalMoves: "<<chessTimer.getTotalCount();
		getchar();
		exit(0);
	}
}



/**
	Gets the all the possible moves for current player in a vector<vector<UI64>> iterable format.
	
	@Author Olli Koskinen, Arttu Nieminen
	@return vector<vector<UI64>> the moves

*/
std::vector<std::vector<UI64>> Board::getLegalMoves(){
	return _position->genLegalMoves(_BitBoards);
}

/**
	Copies the bitboard to a backup for later retrieving.
	
	@Author Olli Koskinen, Arttu Nieminen
*/
UI64 *Board::makeBoardBackUp(){
	UI64* b = new UI64[BITBOARDS];
	b[0]	= 	_BitBoards[0]; 
	b[1]	= 	_BitBoards[1]; 
	b[2]	= 	_BitBoards[2]; 
	b[3]	= 	_BitBoards[3]; 
	b[4]	= 	_BitBoards[4]; 
	b[5]	= 	_BitBoards[5]; 
	b[6]	= 	_BitBoards[6]; 
	b[7]	= 	_BitBoards[7]; 
	b[8]	= 	_BitBoards[8]; 
	b[9]	= 	_BitBoards[9]; 
	b[10]	= 	_BitBoards[10];
	b[11]	= 	_BitBoards[11];
	b[12]	= 	_BitBoards[12];
	b[13]	= 	_BitBoards[13];
	b[14]	= 	_BitBoards[14];
	b[15]	= 	_BitBoards[15];
	b[16]	= 	_BitBoards[16];
	//memcpy(*b, _BitBoards , BITBOARDS);
	return b;
}


/**
	Reversing the moves to original state.
	
	@Author Olli Koskinen, Arttu Nieminen
*/
void Board::takeBack(UI64 *_backUp){
	_BitBoards[0] =  _backUp[0];
	_BitBoards[1] =  _backUp[1];
	_BitBoards[2] =  _backUp[2];
	_BitBoards[3] =  _backUp[3];
	_BitBoards[4] =  _backUp[4];
	_BitBoards[5] =  _backUp[5];
	_BitBoards[6] =  _backUp[6];
	_BitBoards[7] =  _backUp[7];
	_BitBoards[8] =  _backUp[8];
	_BitBoards[9] =  _backUp[9];
	_BitBoards[10] = _backUp[10];
	_BitBoards[11] = _backUp[11];
	_BitBoards[12] = _backUp[12];
	_BitBoards[13] = _backUp[13];
	_BitBoards[14] = _backUp[14];
	_BitBoards[15] = _backUp[15];
	_BitBoards[16] = _backUp[16];

		_position->setToMove(_position->getToMove() == WHITE ? BLACK : WHITE );
	//BitBoardToMoves();
	//memcpy(_BitBoards, _backUp, BITBOARDS);
}


/**
	Function for AI to make false moves
	
	@Author Olli Koskinen, Arttu Nieminen
*/
void Board::makeMove(std::vector<UI64> move){

	UI64 source = move.at(0);
	UI64 dest = move.at(1);
	int toMove = _position->getToMove();
	int enemyPieces = toMove == WHITE ? B_PIECES : W_PIECES;
	int ourPieces	= toMove == WHITE ? W_PIECES : B_PIECES;


	if(toMove == WHITE){
		//If we promote
		if(((dest & EIGHT_RANK) != 0 ) && ((source & _BitBoards[ W_PAWN ]) != 0))
		{
			//Set the the dest as a queen, always
			_BitBoards[ W_QUEEN ] |= dest;

			//Remove the dest from enemy pieces and from our ownF
			_BitBoards[ W_PAWN   ] &= ~source;
		}
		if((source & _BitBoards[ W_KING ]) != 0){
			//If the king moves right 2 squares, it's castling
			if((dest == (source<<2)) && (_BitBoards[W_ROOK] & _SquareBits[7]) != 0 &&((source & _SquareBits[4]) != 0)){
				//Move the king
				_BitBoards[ W_KING ] &=  ~source;
				_BitBoards[ W_KING ] |=  dest;

				//Move the rook
				UI64 rooksPlace =(_BitBoards[ W_ROOK ] ^(_BitBoards[ W_ROOK ] & -_BitBoards[ W_ROOK ]));
				_BitBoards[ W_ROOK ] &=  ~ (_BitBoards[ W_ROOK ] ^(_BitBoards[ W_ROOK ] & -_BitBoards[ W_ROOK ]));
				_BitBoards[ W_ROOK] |=   rooksPlace  >> 2 ;

			}
			else if((dest == (source>>2) )&&  (_BitBoards[W_ROOK] & _SquareBits[0]) != 0 &&((source & _SquareBits[4]) != 0)){ //same as above but right
				_BitBoards[ W_KING ] &=  ~source;
				_BitBoards[ W_KING ] |=  dest;

				//Delete the piece we are going to move and then add it to a new place
				UI64 rooksPlace = (_BitBoards[ W_ROOK ] & -_BitBoards[ W_ROOK ]);
				_BitBoards[ W_ROOK ] &=  ~(_BitBoards[ W_ROOK ] & -_BitBoards[ W_ROOK ]);
				_BitBoards[ W_ROOK ] |=   rooksPlace << 3;
			}
		}
		//En Passant

		if((source & _BitBoards[ W_PAWN ]) != 0){
			if( dest == (source << 16) ){ //If we do a doublepush with pawn
				_BitBoards[ ENPASSANT ] |= source << 8; 
			}
		}

	}else{

		//If we promote
		if(((dest & FIRST_RANK) != 0 ) && ((source & _BitBoards[ B_PAWN ]) != 0))
		{
			//Set the the dest as a queen, always
			_BitBoards[ B_QUEEN ] |= dest;

			//Remove the dest from enemy pieces and from our own
			_BitBoards[ B_PAWN   ] &= ~source;
		}
		if((source & _BitBoards[ B_KING ]) != 0){
			//If the king moves right 2 squares, it's castling
			if((dest == (source<<2)) && ( _BitBoards[B_ROOK] & _SquareBits[63]) != 0&&((source & _SquareBits[60]) != 0)){
				//Move the king
				_BitBoards[ B_KING ] &=  ~source;
				_BitBoards[ B_KING ] |=  dest;

				//Move the rook
				UI64 rooksPlace =(_BitBoards[ B_ROOK ] ^(_BitBoards[ B_ROOK ] & -_BitBoards[ B_ROOK ]));
				_BitBoards[ B_ROOK ] &=  ~ (_BitBoards[ B_ROOK ] ^(_BitBoards[ B_ROOK ] & -_BitBoards[ B_ROOK ]));
				_BitBoards[ B_ROOK] |=   rooksPlace  >> 2 ;

			}
			else if((dest == (source>>2) ) && ( _BitBoards[B_ROOK] & _SquareBits[56]) != 0&&((source & _SquareBits[60]) != 0)){ //same as above but right
				_BitBoards[ B_KING ] &=  ~source;
				_BitBoards[ B_KING ] |=  dest;

				//Delete the piece we are going to move and then add it to a new place
				UI64 rooksPlace = (_BitBoards[ B_ROOK ] & -_BitBoards[ B_ROOK ]);
				_BitBoards[ B_ROOK ] &=  ~(_BitBoards[ B_ROOK ] & -_BitBoards[ B_ROOK ]);
				_BitBoards[ B_ROOK ] |=   rooksPlace << 3;
			}
		}
		//En Passant

		if((source & _BitBoards[ B_PAWN ]) != 0){
			if( (dest == source >> 16) ){ //If we do a doublepush with pawn
				_BitBoards[ ENPASSANT ] |= source >> 8; 
			}
		}
	}

	//Attacks
	//If there is enemy in the dest square
	if( (_BitBoards[ enemyPieces ]  &  dest) != 0 ){
		int i	= toMove == WHITE ? B_KING : W_KING;
		int end = toMove == WHITE ? B_PAWN : W_PAWN;
		//And just in case we search every enemy table trhough if there's still someone in our dest square
		for( i; i <= end; i++){
			if(( _BitBoards[ i ] & dest) != 0 ){
				_BitBoards[ i ]  &=  ~dest;
				break;
			}
		}
	}
	else{
		//Delete the piece we are going to move and then add it to a new place
		//If we have enpassant move
		if((_BitBoards[ ENPASSANT ]  & dest) != 0){
			if(toMove == WHITE)
				_BitBoards[ B_PAWN ] &=  ~dest >> 8;
			else
				_BitBoards[ W_PAWN ] &=  ~dest << 8;
		}


		int i	= toMove == WHITE ? W_KING : B_KING;
		int end = toMove == WHITE ? W_PAWN : B_PAWN;
		//update the dest and source tables
		for(i; i<end; i++){
			if(( _BitBoards[ i ] & source) != 0 ){
				_BitBoards[ i ]   &=  ~source;
				_BitBoards[ i ]  |=  dest;
				break;
			}
		}
	}

	//Update the all the white/black pieces according to turn
	_BitBoards[ W_PIECES ]  = _BitBoards[ W_PAWN ] | _BitBoards[ W_ROOK ] 
	| _BitBoards[ W_KNIGHT ] | _BitBoards[ W_BISHOP ]  | _BitBoards[ W_QUEEN ]  | _BitBoards[ W_KING ];

	_BitBoards[ B_PIECES ]  = _BitBoards[ B_PAWN ] | _BitBoards[ B_ROOK ] 
	| _BitBoards[ B_KNIGHT ] | _BitBoards[ B_BISHOP ]  | _BitBoards[ B_QUEEN ]  | _BitBoards[ B_KING ];


	_BitBoards[ EMPTYSQUARES ]= ~( _BitBoards[ W_PIECES ] | _BitBoards[ B_PIECES ] );

	//BitBoardToMoves();
	//Update enpassant table
	_BitBoards[ ENPASSANT ] &= toMove == WHITE ? ~SIXTH_RANK : ~THIRD_RANK;
	_position->setToMove(_position->getToMove() == WHITE ? BLACK : WHITE );
}


/**
	Gets the all the possible moves for current player in a vector<vector<UI64>> iterable format.
	
	@Author Olli Koskinen, Arttu Nieminen
	@return vector<vector<UI64>> the moves

*/
std::vector<std::string> Board::getMoveStrings(){
	std::vector<std::string> strVector;
	std::string str;
	std::vector<std::vector<UI64>> bitboards = getLegalMoves();

	if(bitboards.size() == 0){

		if(_position->getToMove() == WHITE){
			if(_position->wIsCheck(_BitBoards)){
			//	system("CLS");
				PlaySound(L"gameover.wav",NULL,SND_FILENAME|SND_ASYNC); 
				std::cout<<"Checkmate!\nThe game ends in favor of Black!";
				std::cout<<"\ngenLegalMoves average time was: "<<chessTimer.getAverage()<<" microseconds";
				std::cout<<"\nTimes called genLegalMoves: "<<chessTimer.getTotalCount();
				getchar();
				exit(0);
			}
		}else{

			if(_position->bIsCheck(_BitBoards)){
				//system("CLS");
				PlaySound(L"gameover.wav",NULL,SND_FILENAME|SND_ASYNC); 
				std::cout<<"Checkmate!\nThe game ends in favor of White!";
				std::cout<<"\ngenLegalMoves average time was: "<<chessTimer.getAverage()<<" microseconds";
				std::cout<<"\nTimes called genLegalMoves: "<<chessTimer.getTotalCount();
				getchar();
				exit(0);
			}
		}

		//system("CLS");
		PlaySound(L"gameover.wav",NULL,SND_FILENAME|SND_ASYNC); 
		std::cout<<"Stalemate!\nThe game ends in draw!";
		std::cout<<"\ngenLegalMoves average time was: "<<chessTimer.getAverage()<<" microseconds";
		std::cout<<"\nTimes called genLegalMoves: "<<chessTimer.getTotalCount();
		getchar();
		exit(0);
	}

	bool pointIsSet = false;

	int x = 0,
		y = 0,
		x2 = 0,
		y2 = 0;

	//For each source piece we look every possible move in list
	for(int i = 0; i < bitboards.size(); i++){

		int j = bitScanForward( bitboards.at(i).at(0) );
		int k = bitScanForward( bitboards.at(i).at(1) );

		int x =  j & 7;
		int y =  j >> 3; 

		int x2 =  k & 7;
		int y2 =  k >> 3; 

		strVector.insert(strVector.end(), movesAsString(x,y,x2,y2)), pointIsSet = false;

	}
	return strVector;
}
/**
	Prints the move as a human-readable string from given two points.

	@author Christoffer Niska, Mikko Malmari, Olli Koskinen, Arttu Nieminen
	@retrun std::string
*/
std::string Board::movesAsString(int x1, int y1, int x2, int y2){

	std::string str;
	str += LETTERS[x1];
	str += '0' + (y1 +1 ); // amazing logic! Modified by Olli, The +1 is because we subtracted 1 
	str += '-';			  //from the y coord for better indexin with arrays
	str += LETTERS[x2];
	str += '0' + (y2 + 1); // amazing logic again! And a gain, same as above.
	return str;
}


Square *Board::getSquareAt(int x, int y)
{
	// Validate the coordinates.
	if (x >= 0 && x < 8 && y >= 0 && y < 8)
	{
		return _squares[x][y];
	}
	else
	{
		// Invalid coordinates.
		return NULL;
	}
}

/**
	Returns the piece at the given coordinates.

	@author Christoffer Niska
	@param x the x-coordinate
	@param y the y-coordinate
	@return the piece or NULL if no piece was found
*/
Piece *Board::getPieceAt(int x, int y)
{
	Square *square;
	
	square = getSquareAt(x, y);

	if (square != NULL)
	{
		return square->getPiece();
	}
	else
	{
		return NULL;
	}

	// todo: free mem.
}




/**
	Sets the piece at the given coordinates.

	@author Christoffer Niska
	@param x the x-coordinate
	@param y the y-coordinate
	@return void
*/
void Board::setPieceAt(int x, int y, Piece *piece)
{
	Square *square;

	square = getSquareAt(x, y);

	if (square != NULL)
	{
		square->setPiece(piece);
	}

	// todo: free mem.
}

void Board::setPosition(Position *position)
{
	_position = position;
}

int Board::alphaBetaMax( int alpha, int beta, int depth ) {
	if ( depth == 0 ) return _position->evaluate(_BitBoards);
	std::vector<std::vector<UI64>> moveVector = _position->genLegalMoves(_BitBoards);
	int i = 0; int score = 0;
	for (i=0;i<moveVector.size();i++) { //Go through every node in moveVector
		//Backup the original state
		UI64 *backuP = makeBoardBackUp();

		makeMove(moveVector.at(i));  
		score = alphaBetaMin( alpha, beta, depth - 1 );
		takeBack(backuP);
		delete backuP;
		if( score >= beta )
			return beta;  
		if( score > alpha )
			alpha = score;

	}
	return alpha;
}

int Board::alphaBetaMin( int alpha, int beta, int depth ) {
	if ( depth == 0 ) return _position->evaluate(_BitBoards);
	std::vector<std::vector<UI64>> moveVector = _position->genLegalMoves(_BitBoards);
	int i = 0; int score = 0;
	for ( i=0;i<moveVector.size();i++) {
		UI64 *backuP = makeBoardBackUp();
		makeMove(moveVector.at(i));
		score = alphaBetaMax( alpha, beta, depth - 1 );

		takeBack(backuP);
		delete backuP;
		if( score <= alpha )
			return alpha; 
		if( score < beta )
			beta = score; 
	}

	return beta;
}

Move *Board::wRootSearch() {
	int best = 0; int score = 0;
	std::vector<UI64> bestMove;
	std::vector<std::vector<UI64>> moveVector = _position->genLegalMoves(_BitBoards);
	int i; 
	std::cout<<"moveVector.size() == "<<moveVector.size()<<std::endl;
	for ( i=0;i<moveVector.size();i++) {
		UI64 *backuP = makeBoardBackUp();
		makeMove(moveVector.at(i));

		score = alphaBetaMin(INT_MIN, INT_MAX, 4);
		std::cout<<"score: "<<score<<std::endl;
		takeBack(backuP);
		delete backuP;
		if(score >= best){

			bestMove = moveVector.at(i);
		}
	}
	return BitBoardToMoves(bestMove);
}

Move *Board::bRootSearch(){
	int best = 0;int score = 0;
	std::vector<UI64> bestMove;
	std::vector<std::vector<UI64>> moveVector = _position->genLegalMoves(_BitBoards);
	int i; 
	std::cout<<"moveVector.size() == "<<moveVector.size()<<std::endl;
	for ( i=0;i<moveVector.size();i++) {
		UI64 *backuP = makeBoardBackUp();
		makeMove(moveVector.at(i));
		score = alphaBetaMax(INT_MIN, INT_MAX, 4);
		std::cout<<"score: "<<score<<std::endl;
		takeBack(backuP);
		delete backuP;
		if(score <= best){
			bestMove = moveVector.at(i);
		}
	}
	return BitBoardToMoves(bestMove);
}