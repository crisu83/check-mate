#include "stdafx.h"


//A least significant table for bitscan courtecy of Walter Faxon
const char LSB_64_table[154] =
{
#define __ 0
   22,__,__,__,30,__,__,38,18,__, 16,15,17,__,46, 9,19, 8, 7,10,
   0, 63, 1,56,55,57, 2,11,__,58, __,__,20,__, 3,__,__,59,__,__,
   __,__,__,12,__,__,__,__,__,__, 4,__,__,60,__,__,__,__,__,__,
   __,__,__,__,21,__,__,__,29,__, __,37,__,__,__,13,__,__,45,__,
   __,__, 5,__,__,61,__,__,__,53, __,__,__,__,__,__,__,__,__,__,
   28,__,__,36,__,__,__,__,__,__, 44,__,__,__,__,__,27,__,__,35,
   __,52,__,__,26,__,43,34,25,23, 24,33,31,32,42,39,40,51,41,14,
   __,49,47,48,__,50, 6,__,__,62, __,__,__,54
#undef __
};




/**
	Constructor.
 */
Board::Board(void)
{
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
	historyIndex = 0;

	resetPerftCounters();

	for(int i = 0; i<  10 ; i++){
		historyTable[i] = 0;
	}

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
	_BitBoards[ CASTLING ] = 0x8100000000000081;


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


void Board::superHiddenRenderEmptySquares(int board){
	UI64 value;
	switch(board){

	case 30:
		value = _position->wAttacks(_BitBoards);
		break;
	case 40:
		value = _position->bAttacks(_BitBoards);
		break;
	case 50:{
		value = ~_BitBoards[EMPTYSQUARES];
		break;}
	default:
		if(board >= 0 && board <= 16)
			value = _BitBoards[board];
		else
			value = _BitBoards[0];
		break;
		}

	std::string tempString = "";

   for (int i = SQUARES -1 ; i>=0; i-- ) 
   {
	   tempString.append((((value & _SquareBits[i]) == 0) ? "0" : "1"));
      if ( i % 8 == 0 ){
		  std::reverse(tempString.begin(),tempString.end());
         std::cout <<tempString<<std::endl;
		 tempString ="";
	  }
   }
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
	int j ;
	UI64 tmpMoves;
	for(int i = 1; i <= B_PAWN; i++){
		tmpMoves = _BitBoards[i];
		while((tmpMoves & -tmpMoves) != 0){
			//j = bitScanForward(tmpMoves & -tmpMoves);
			j = bitScanForward(tmpMoves & -tmpMoves);
				int x =  j & 7;
				int y =  j >> 3;
				setPieceAt(x,y, new Piece(i));
				tmpMoves &= tmpMoves-1;
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
 * @author Walter Faxon, slightly modified
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
int Board::bitScanForward(UI64 bb)
{
   unsigned int t32;
   assert(bb);
   bb  ^= bb - 1;
   t32  = (int)bb ^ (int)(bb >> 32);
   t32 ^= 0x01C5FC81;
   t32 +=  t32 >> 16;
   t32 -= (t32 >> 8) + 51;
   return LSB_64_table [t32 & 255]; // 0..63
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
		_BitBoards[CASTLING] &= ~_SquareBits[0];
		_BitBoards[CASTLING] &= ~_SquareBits[7];
	}
	else if(type == B_KING){
		_BitBoards[CASTLING] &= ~_SquareBits[56];
		_BitBoards[CASTLING] &= ~_SquareBits[63];
	}


	//If we move one of the rooks, then it will not be able to castle anymore
	if(type == W_ROOK){
		//LS1B
		if((_SquareBits[sourceIndex] & _SquareBits[0]) ) //White queen side rook
			_BitBoards[CASTLING] &= ~_SquareBits[0];
		//Reseted LS1B
		if( ( _SquareBits[sourceIndex] & _SquareBits[7])) //White king side rook
			_BitBoards[CASTLING] &= ~_SquareBits[7];
	}
	else if(type == B_ROOK){
		if(_SquareBits[sourceIndex] & _SquareBits[63] ) //Black queen side rook
			_BitBoards[CASTLING] &= ~_SquareBits[63];
		if(  _SquareBits[sourceIndex] & _SquareBits[56])  //Black king side rook
			_BitBoards[CASTLING] &= ~_SquareBits[56];
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
	if( (_BitBoards[ enemyPieces ]  &  _SquareBits[ destIndex ]) != 0 ){

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
			if((_BitBoards[ ENPASSANT ]  & _SquareBits[destIndex]) != 0   ){
				if(toMove == WHITE && (_SquareBits[sourceIndex] & _BitBoards[ W_PAWN ]) != 0)
					_BitBoards[ B_PAWN ] &=  ~(_SquareBits[ destIndex ] >> 8);
				else if((toMove == BLACK && (_SquareBits[sourceIndex] & _BitBoards[ B_PAWN ]) != 0))
					_BitBoards[ W_PAWN ] &=  ~(_SquareBits[ destIndex ] << 8);
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

	if(getSquareAt(_curMove->getX1(),_curMove->getY1()) == NULL  || getSquareAt(_curMove->getX1(),_curMove->getY1())->isEmpty()){
		return false;
	}

	if(getPieceAt(_curMove->getX1(),_curMove->getY1())-> getType() == 0)
		return false;

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
		UI64 source		  = move[i].at(0);
		UI64 destinations = move[i].at(1);

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
	b[17]   =   _BitBoards[17];

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
	_BitBoards[17] = _backUp[17];

	historyIndex--;
		_position->setToMove(_position->getToMove() == WHITE ? BLACK : WHITE );
	//BitBoardToMoves();
	//memcpy(_BitBoards, _backUp, BITBOARDS);
}


/**
	Function for AI to make false moves
	
	@Author Olli Koskinen, Arttu Nieminen
*/
void Board::makeMove(std::vector<UI64> move)
{
	
	UI64 source = move[0];
	UI64 dest   = move[1];
	int toMove  = _position->getToMove();

	//If the type is one of the Whites, then we look from black pieces
	//And if the type is one of the blacks, then we look from white pieces
	int enemyPieces = toMove == WHITE ? B_PIECES : W_PIECES;
	int ourPieces	= toMove == WHITE ? W_PIECES : B_PIECES;

	//We find what we are against at and with what we are moving
	int ourType = 0;
	int enemyType = 0;

	
	if(toMove == WHITE){
		if(_position->wIsCheck(_BitBoards))
			cheks++;
	}else{
		if(_position->bIsCheck(_BitBoards))
			cheks++;
	}

	int i = 1;
	int end = B_PAWN;

	for(i; i<=end; i++)
	{
		if(( _BitBoards[ i ] & source) != 0 )
		{
			ourType = i;
		}
		else if((_BitBoards[ i ] & dest) != 0)
		{
			enemyType = i;
		}

		if(enemyType !=0 && ourType != 0)
		{
			break;
		}
	}

	//If we move the king, we cant do castling anymore
	if(ourType == W_KING){
		_BitBoards[CASTLING] &= ~_SquareBits[0];
		_BitBoards[CASTLING] &= ~_SquareBits[7];
	}
	else if(ourType == B_KING){
		_BitBoards[CASTLING] &= ~_SquareBits[56];
		_BitBoards[CASTLING] &= ~_SquareBits[63];
	}

	//If we move one of the rooks, then it will not be able to castle anymore
	if(ourType == W_ROOK){
		//LS1B
		if((source & _SquareBits[0]) ) //White queen side rook
			_BitBoards[CASTLING] &= ~_SquareBits[0];
		//Reseted LS1B
		if( ( source & _SquareBits[7])) //White king side rook
			_BitBoards[CASTLING] &= ~_SquareBits[7];
	}
	else if(ourType == B_ROOK){
		if(source & _SquareBits[63] ) //Black queen side rook
			_BitBoards[CASTLING] &= ~_SquareBits[63];
		if(  source & _SquareBits[56])  //Black king side rook
			_BitBoards[CASTLING] &= ~_SquareBits[56];
	}

	historyTable[historyIndex] = 0;
	//Attacks
	//If there is enemy in the dest square
	if(_BitBoards[ enemyPieces ] & dest)
	{	
		//If we promote
		if((dest & EIGHT_RANK) &&(source & _BitBoards[ W_PAWN ]))			
		{
			promotions++;
			//Set the dest as a queen, always
			historyTable[historyIndex] = 0;
			historyTable[historyIndex] +=PROMO;
			_BitBoards[ W_QUEEN ] |= dest;
		}
		else if((dest & FIRST_RANK )&&(source & _BitBoards[ B_PAWN ]))
		{
			promotions++;
			//Set the the dest as a queen, always
			historyTable[historyIndex] = 0;
			historyTable[historyIndex] +=PROMO;
			_BitBoards[ B_QUEEN ] |= dest;
		}
		else
		{
			_BitBoards[ ourType	  ]  |= dest;    //add our type to dests
		}

		captures++;
		historyTable[historyIndex] +=CAPT + enemyType;
		_BitBoards[ enemyType ]  &= ~dest;   //Remove the enemy from our dest
		_BitBoards[ ourType	  ]  &= ~source; //remove our piece from its source
	}
	else   //Non attacking moves. We read en passant as a non attacking, since we dont directly put our dest to enemy piece
	{			
		historyTable[historyIndex] = 0;
		if(toMove == WHITE)
		{
			//En Passant
			if(source & _BitBoards[ W_PAWN ])
			{
				if( dest == (source << 16) )//If we do a doublepush with pawn
				{ 
					_BitBoards[ ENPASSANT ] |= source << 8; 
				}
			}
			//CASTLING
			if((source & _BitBoards[ W_KING ]) &&(source & _SquareBits[4]))
			{
				//If the king moves right 2 squares, it's castling
				if((dest == (source<<2)) && (_BitBoards[W_ROOK] & _SquareBits[7]) )
				{
					castlings++;
					historyTable[historyIndex] = 0;
					historyTable[historyIndex] +=CASTL;
					//Move the king
					_BitBoards[ W_KING ] &=  ~source;
					_BitBoards[ W_KING ] |=  dest;

					//Move the rook
					UI64 rooksPlace =(_BitBoards[ W_ROOK ] ^(_BitBoards[ W_ROOK ] & -_BitBoards[ W_ROOK ]));
					_BitBoards[ W_ROOK ] &=  ~ (_BitBoards[ W_ROOK ] ^(_BitBoards[ W_ROOK ] & -_BitBoards[ W_ROOK ]));
					_BitBoards[ W_ROOK ] |=   rooksPlace  >> 2 ;
				}
				else if((dest == (source>>2) )&&(_BitBoards[W_ROOK] & _SquareBits[0]))//same as above but right
				{ 
					castlings++;
					historyTable[historyIndex] = 0;
					historyTable[historyIndex] +=CASTL;
					_BitBoards[ W_KING ] &=  ~source;
					_BitBoards[ W_KING ] |=  dest;

					//Delete the piece we are going to move and then add it to a new place
					UI64 rooksPlace = (_BitBoards[ W_ROOK ] & -_BitBoards[ W_ROOK ]);
					_BitBoards[ W_ROOK ] &=  ~(_BitBoards[ W_ROOK ] & -_BitBoards[ W_ROOK ]);
					_BitBoards[ W_ROOK ] |=   rooksPlace << 3;
				}
			}

			//PROMOTE
			else if((dest & EIGHT_RANK) && (source & _BitBoards[ W_PAWN ]))			
			{
				promotions++;
				historyTable[historyIndex] = 0;
				historyTable[historyIndex] +=PROMO;
				//Set the the dest as a queen, always
				_BitBoards[ W_QUEEN ] |= dest;
				//Remove the dest from enemy pieces and from our ownF
				_BitBoards[ W_PAWN  ] &= ~source;
			}
			else
			{
				//If we have enpassant move
				//Delete the piece we are going to move and then add it to a new place
				if((_BitBoards[ ENPASSANT ]  & dest)&& (ourType == W_PAWN))
				{
					enpassants++;
					historyTable[historyIndex] = 0;
					historyTable[historyIndex] +=ENPASS;
					_BitBoards[ B_PAWN ] &=  ~(dest >> 8);
				}
				//We are doing a normal move
				//update the dest and source tables
				_BitBoards[ ourType	  ]  |= dest;    //add our type to dest
				_BitBoards[ ourType	  ]  &= ~source; //remove our piece from its source
			}
		}
		else
		{
			//En Passant
			if(source & _BitBoards[ B_PAWN ]){
				if( (dest == source >> 16) ){ //If we do a doublepush with pawn
					_BitBoards[ ENPASSANT ] |= source >> 8; 
				}
			}
			if((source & _BitBoards[ B_KING ])&&(source & _SquareBits[60]) )
			{
				//If the king moves right 2 squares, it's castling
				if((dest == (source<<2))&&( _BitBoards[B_ROOK] & _SquareBits[63]))
				{
					castlings++;
					historyTable[historyIndex] = 0;
					historyTable[historyIndex] +=CASTL;
					//Move the king
					_BitBoards[ B_KING ] &=  ~source;
					_BitBoards[ B_KING ] |=  dest;

					//Move the rook
					UI64 rooksPlace =(_BitBoards[ B_ROOK ] ^(_BitBoards[ B_ROOK ] & -_BitBoards[ B_ROOK ]));
					_BitBoards[ B_ROOK ] &=  ~ (_BitBoards[ B_ROOK ] ^(_BitBoards[ B_ROOK ] & -_BitBoards[ B_ROOK ]));
					_BitBoards[ B_ROOK] |=   rooksPlace  >> 2 ;

				}
				else if((dest == (source>>2) ) && ( _BitBoards[B_ROOK] & _SquareBits[56]))//same as above but right
				{ 
					castlings++;
					historyTable[historyIndex] = 0;
					historyTable[historyIndex] +=CASTL;
					_BitBoards[ B_KING ] &=  ~source;
					_BitBoards[ B_KING ] |=  dest;

					//Delete the piece we are going to move and then add it to a new place
					UI64 rooksPlace = (_BitBoards[ B_ROOK ] & -_BitBoards[ B_ROOK ]);
					_BitBoards[ B_ROOK ] &=  ~(_BitBoards[ B_ROOK ] & -_BitBoards[ B_ROOK ]);
					_BitBoards[ B_ROOK ] |=   rooksPlace << 3;
				}
			}
			else if((dest & FIRST_RANK) && (source & _BitBoards[ B_PAWN ]))
			{
				promotions++;
				historyTable[historyIndex] = 0;
				historyTable[historyIndex] +=PROMO;
				//Set the the dest as a queen, always
				_BitBoards[ B_QUEEN ] |= dest;
				//Remove the dest from enemy pieces and from our own
				_BitBoards[ enemyType ] &= ~dest;
				_BitBoards[ B_PAWN	  ] &= ~source;
			}
			else
			{ 
				//If we have enpassant move
				//Delete the piece we are going to move and then add it to a new place
				if((_BitBoards[ ENPASSANT ] & dest)  && (ourType == B_PAWN))
				{
					enpassants++;
					historyTable[historyIndex] = 0;
					historyTable[historyIndex] +=ENPASS;
					_BitBoards[ W_PAWN ] &=  ~(dest << 8);
				}
				//We are doing a normal move
				//update the dest and source tables
				_BitBoards[ ourType	  ]  |= dest;    //add our type to dest
				_BitBoards[ ourType	  ]  &= ~source; //remove our piece from its source
			}
		}
	}

	//Update the all the white/black pieces according to turn
	_BitBoards[ W_PIECES ]  = _BitBoards[ W_PAWN ] | _BitBoards[ W_ROOK ] 
	| _BitBoards[ W_KNIGHT ] | _BitBoards[ W_BISHOP ]  | _BitBoards[ W_QUEEN ]  | _BitBoards[ W_KING ];

	_BitBoards[ B_PIECES ]  = _BitBoards[ B_PAWN ] | _BitBoards[ B_ROOK ] 
	| _BitBoards[ B_KNIGHT ] | _BitBoards[ B_BISHOP ]  | _BitBoards[ B_QUEEN ]  | _BitBoards[ B_KING ];


	_BitBoards[ EMPTYSQUARES ]= ~( _BitBoards[ W_PIECES ] | _BitBoards[ B_PIECES ] );

	//Update enpassant table
	_BitBoards[ ENPASSANT ] &= toMove == WHITE ? ~SIXTH_RANK : ~THIRD_RANK;

		
	_position->setToMove(_position->getToMove() == WHITE ? BLACK : WHITE );
	historyIndex++;
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

	//For each source piece we look every possible move in list
	for(int i = 0; i < bitboards.size(); i++){

		int j = bitScanForward( bitboards[i].at(0) );
		int k = bitScanForward( bitboards[i].at(1) );

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

/**
	Prints the move as a human-readable string from bitboards.

	@author Christoffer Niska, Mikko Malmari, Olli Koskinen, Arttu Nieminen
	@retrun std::string
*/
std::string Board::movesAsString(std::vector<UI64> move){
	int x1,y1,x2,y2;
	int j;
	j = bitScanForward(move.at(0));

		 x1 =  j & 7;
		 y1 =  j >> 3; 

		 j = bitScanForward(move.at(1));
		 x2 =  j & 7;
		 y2 =  j >> 3; 

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

	if (square != NULL && !square->isEmpty())
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

double Board::alphaBetaMax( double alpha, double beta, int depth ) {
	if ( depth == 0 ) return _position->evaluate(_BitBoards);
	std::vector<std::vector<UI64>> moveVector = _position->genLegalMoves(_BitBoards);
	if(moveVector.size() == 0 && _position->wIsCheck(_BitBoards)) return INT_MIN;
	int i = 0; double score = 0;
	for (i=0;i<moveVector.size();i++) { //Go through every node in moveVector
		//Backup the original state
		UI64 *backuP = makeBoardBackUp();

		makeMove(moveVector[i]);  
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

double Board::alphaBetaMin( double alpha, double beta, int depth ) {
	if ( depth == 0 ) return _position->evaluate(_BitBoards);
	std::vector<std::vector<UI64>> moveVector = _position->genLegalMoves(_BitBoards);
	if(moveVector.size() == 0 && _position->bIsCheck(_BitBoards)) return INT_MAX;
	int i = 0; double score = 0;
	for ( i=0;i<moveVector.size();i++) {
		UI64 *backuP = makeBoardBackUp();
		makeMove(moveVector[i]);
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
	double best = 0; double score = 0;
	std::vector<UI64> bestMove;
	std::vector<std::vector<UI64>> moveVector = _position->genLegalMoves(_BitBoards);
	int i; 
	for ( i=0;i<moveVector.size();i++) 
	{

		UI64 *backuP = makeBoardBackUp();
		makeMove(moveVector[i]);

		score = alphaBetaMin(INT_MIN, INT_MAX, 4);
		if(i == 0)
			best = score;  //We need a reference point

		takeBack(backuP);
		delete backuP;

		if(score >= best){
			best = score;
			bestMove = moveVector[i];

		}
	}
	std::cout<<"The Score of this move: "<<best<<"\n";
	return BitBoardToMoves(bestMove);
}

Move *Board::bRootSearch(){
	double best = 0; double score = 0;
	std::vector<UI64> bestMove;
	std::vector<std::vector<UI64>> moveVector = _position->genLegalMoves(_BitBoards);
	int i; 

	for ( i = 0 ; i < moveVector.size(); i++ ) {
		UI64 *backuP = makeBoardBackUp();
		makeMove(moveVector[i]);
		score = alphaBetaMax(INT_MIN, INT_MAX, 4);

		if(i == 0)
			best = score;  //We need a reference point

		takeBack(backuP);
		delete backuP;
		if(score <= best){
			best = score;
			bestMove = moveVector[i];

		}
	}
	std::cout<<"The Score of this move: "<<best<<"\n";
	return BitBoardToMoves(bestMove);
}

/**
	Perft function counts all the possible moves to a certain depth

	@author Olli Koskinen, Arttu Nieminen
	@param int the depth to which ply we go to
	@return the number of possible moves.
*/

UI64 Board::Perft(int depth)
{
    int n_moves, i;
   UI64 nodes = 0;
	std::vector<std::vector<UI64>> moveVector = _position->genLegalMoves(_BitBoards);
	n_moves = moveVector.size();
	if(n_moves == 0){
		if(_position->getToMove() == WHITE && _position->wIsCheck(_BitBoards)){
		checkmates++;
		}
		else if(_position->getToMove() == BLACK && _position->bIsCheck(_BitBoards)){
			checkmates++;
		}
	}

    if (depth == 0) return 1;
    for (i = 0; i < n_moves; i++) {
		UI64 *backuP = makeBoardBackUp();
        makeMove(moveVector[i]);
        nodes += Perft(depth - 1);
		//unMake(moveVector[i]);
		takeBack(backuP);
		delete backuP;
    }
    return nodes;
}

void Board::PerftResults(){
	std::cout<<"Captures: "<<captures<<std::endl;
	std::cout<<"En Passants: "<<enpassants<<std::endl;
	std::cout<<"Castlings: "<<castlings<<std::endl;
	std::cout<<"Checks: "<<cheks<<std::endl;
	std::cout<<"CheckMates: "<<checkmates<<std::endl;

	resetPerftCounters();
}

void Board::resetPerftCounters(){
	captures = 0, enpassants = 0,promotions = 0, cheks =0, checkmates =0 ,castlings = 0;
};



/**
	A function that counts all the child moves from the first depth and adds them to a list
	for DEBUGging purposes.
	@author Olli Koskinen, Arttu Nieminen
*/
void Board::divided(int depth){
	 int n_moves, i;
   UI64 nodes = 0;
	std::vector<std::vector<UI64>> moveVector = _position->genLegalMoves(_BitBoards);
	n_moves = moveVector.size();
	if(depth == 0) return;
    for (i = 0; i < n_moves; i++) {
		UI64 *backuP = makeBoardBackUp();
        makeMove(moveVector[i]);
		UI64 b =divide(depth - 1);
		std::cout<<movesAsString(moveVector[i])<<": "<< b<<"\n";
		nodes +=b;
        takeBack(backuP);
		delete backuP;
    }
	std::cout<<"Moves: "<<n_moves<<"\nNodes: "<<nodes<<std::endl;
}

UI64 Board::divide(int depth){
	 int n_moves, i;
   UI64 nodes = 0;
	std::vector<std::vector<UI64>> moveVector = _position->genLegalMoves(_BitBoards);
	n_moves = moveVector.size();

	if(depth == 0) return 1;
    for (i = 0; i < n_moves; i++) {
		UI64 *backuP = makeBoardBackUp();
        makeMove(moveVector[i]);
		nodes+= divide(depth - 1);
        takeBack(backuP);
		delete backuP;
    }
	return nodes;
}


void Board::unMake(std::vector<UI64> move){
	historyIndex--;
	_position->setToMove(_position->getToMove() == WHITE ? BLACK : WHITE );

	UI64 source = move.at(0);
	UI64 dest = move.at(1);
	int toMove = _position->getToMove();

	//If the type is one of the Whites, then we look from black pieces
	//And if the type is one of the blacks, then we look from white pieces
	int enemyPieces = toMove == WHITE ? B_PIECES : W_PIECES;
	int ourPieces	= toMove == WHITE ? W_PIECES : B_PIECES;

	//We find what we are against at and with what we are moving
	int ourType = 0;
	int enemyType = 0;

	int i = 1;
	int end = B_PAWN;

	for(i; i<=end; i++)
	{
		if(( _BitBoards[ i ] & dest) != 0 )
		{
			ourType = i;
			break;
		}
	}

	if(historyTable[historyIndex] > PROMO+CAPT){

		enemyType = historyTable[historyIndex] - (PROMO+CAPT);
		historyTable[historyIndex] = 0;
		if(toMove ==WHITE){
			//remove the queen from the dest
			_BitBoards[ W_QUEEN ] &= ~dest;
		}
		else
		{
			//remove the queen from the dest
			_BitBoards[ B_QUEEN ] &= ~dest;
		}
		_BitBoards[ enemyType ] |= dest; 
		_BitBoards[ ourType	  ]  |= source; //remove our piece from its source

	}else if (historyTable[historyIndex] >= CAPT && historyTable[historyIndex] < PROMO){

		enemyType = historyTable[historyIndex] - (CAPT);
		historyTable[historyIndex] = 0;
		_BitBoards[ ourType	  ]  |= source;    
		_BitBoards[ enemyType ]  |= dest;   
		_BitBoards[ ourType	  ]  &= ~dest; 

	}else
	{
		switch(historyTable[historyIndex]){
		case CASTL:{
			historyTable[historyIndex] = 0;
			int rookToMove = toMove == WHITE ? W_ROOK: B_ROOK;
			int kingToMove = toMove == WHITE ? W_KING: B_KING;
			//We HAVE castled, so we look if the rook is on our left side or on our right side to determine which castling we did
			if((_BitBoards[rookToMove] & source>>1) != 0 )
			{

				//Move the king
				_BitBoards[ kingToMove ] &=  ~dest;
				_BitBoards[ kingToMove ] |=  source;

				//Move the rook
				UI64 rooksPlace =(_BitBoards[ rookToMove ] ^(_BitBoards[ rookToMove ] & -_BitBoards[ rookToMove ]));
				_BitBoards[ rookToMove ] &=  ~ (_BitBoards[ rookToMove ] ^(_BitBoards[ rookToMove ] & -_BitBoards[ rookToMove ]));
				_BitBoards[ rookToMove ] |=   rooksPlace  << 2;
			}
			else if((_BitBoards[rookToMove] & source<<1) != 0)//same as above but right
			{ 
				_BitBoards[ kingToMove ] &=  ~dest;
				_BitBoards[ kingToMove ] |=  source;

				//Delete the piece we are going to move and then add it to a new place
				UI64 rooksPlace = (_BitBoards[ rookToMove ] & -_BitBoards[ rookToMove ]);
				_BitBoards[ rookToMove ] &=  ~(_BitBoards[ rookToMove ] & -_BitBoards[ rookToMove ]);
				_BitBoards[ rookToMove ] |=   rooksPlace >> 3;
			}
			break;
					  }
		case ENPASS:{
			historyTable[historyIndex] = 0;
			if(toMove == WHITE)
			{
				_BitBoards[ B_PAWN ] |=  dest >> 8;
				//We are doing a normal move
				//update the dest and source tables
				_BitBoards[ W_PAWN	  ]  &= ~dest;    //Remove our piece from dest
				_BitBoards[ W_PAWN	  ]  |= source; //ad our piece to source
			}
			else{
				_BitBoards[ W_PAWN ] |=  dest << 8;
				//We are doing a normal move
				//update the dest and source tables
				_BitBoards[ B_PAWN	  ]  |= source;    //add our type to dest
				_BitBoards[ B_PAWN	  ]  &= ~dest; //remove our piece from its source


			}
			break;
					   }

		case PROMO:{
			historyTable[historyIndex] = 0;
			if(toMove == WHITE)
			{
				_BitBoards[ W_QUEEN ] &= ~dest;
				_BitBoards[ W_PAWN  ] |= source;
			}
			else
			{
				_BitBoards[ B_QUEEN ] &= ~dest;
				_BitBoards[ B_PAWN  ] |= source;
			}
			break;
			 }

		case 0:{
			historyTable[historyIndex] = 0;
			_BitBoards[ ourType	  ]  |= source;    //add our type to source
			_BitBoards[ ourType	  ]  &= ~dest; //remove our piece from its dest
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

	//Update enpassant table
	_BitBoards[ ENPASSANT ] &= toMove == WHITE ? ~SIXTH_RANK : ~THIRD_RANK;
}
