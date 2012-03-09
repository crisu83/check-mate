#include "StdAfx.h"
#include "Board.h"

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
	@return pointer to move vector
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
	Updates the move to bitboards

	@author Olli Koskinen, Arttu Nieminen
	@return void
*/
void Board::updateBitBoards(Move move, int type){
	//Debug info: 
	if(debug)
		std::cout<<"Bitboards [ type ] = "<<_BitBoards[ type ]<<"\n"; 

	//Find the index for the squareBits array from the coords.
	int sourceIndex =	(move.getX1()) + (move.getY1()<<3);
	int destIndex	=	(move.getX2()) + (move.getY2()<<3);
	int toMove = _position->getToMove();

	//If the type is one of the Whites, then we look from black pieces
	//And if the type is one of the blacks, then we look from white pieces
	int enemyPieces = toMove == WHITE ? B_PIECES : W_PIECES;
	int ourPieces	= toMove == WHITE ? W_PIECES : B_PIECES;

	//Set the fiftyMove rule to zero if pawn is moved, else add +1 on every move.
	if(type == W_PAWN || B_PAWN)
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

	if(debug)
		std::cout<<"source "<<sourceIndex<<" dest "<<destIndex<<"\n";

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
			//And ad a knight to the dest place
			_BitBoards[ B_BISHOP] |=   _SquareBits[ destIndex   ];
			break;		

		case 'q':
			//Queens
			//Remove the pawn from the list
			_BitBoards[ B_PAWN ] &=  ~_SquareBits[ sourceIndex ];
			//And ad a knight to the dest place
			_BitBoards[ B_QUEEN] |=   _SquareBits[ destIndex   ];
			break;

		case 'r':
			//Rooks
			//Remove the pawn from the list
			_BitBoards[ B_PAWN ] &=  ~_SquareBits[ sourceIndex ];
			//And ad a knight to the dest place
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
	Converts the SquareBit to a x,y coordinates

	@author Olli Koskinen, Arttu Nieminen
	@param square the bit representation of move
	@return void    ==    PLXFIXME
*/
void Board::SquareBitToPos(UI64 square){
	int x , y;
	x = square & 7;
	y = square >> 3;
	//plx return meeeee!
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

	std::vector<std::vector<UI64>> move = _position->genLegalMoves(_BitBoards);


	for(int i = 0; i <move.size(); i++){
		UI64 source		  = move.at(i).at(0);
		UI64 destinations = move.at(i).at(1);

		if( (source & _SquareBits[sourceIndex]) == _SquareBits[sourceIndex] ){
			if( (destinations &  _SquareBits[ destIndex ]) == _SquareBits[destIndex] ){
				updateBitBoards(*_curMove, getPieceAt(_curMove->getX1(),_curMove->getY1())-> getType());
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
bool Board::fiftyMoveRule(){
	return fiftyMove >= 50 ? true : false;
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
				std::cout<<"Checkmate!\nThe game ends in favor of Black!";
				getchar();
				exit(0);
			}
		}else{

			if(_position->bIsCheck(_BitBoards)){
				//system("CLS");
				std::cout<<"Checkmate!\nThe game ends in favor of White!";
				getchar();
				exit(0);
			}
		}

		//system("CLS");
		std::cout<<"Stalemate!\nThe game ends in draw!";
		getchar();
		exit(0);
	}

	bool pointIsSet = false;

	int x = 0,
		y = 0,
		x2 = 0,
		y2 = 0;

	//For each source piece we look every possible move in list
	for(int i = 0; i <= bitboards.size()-1; i++){

		UI64 source		  = bitboards.at(i).at(0);
		UI64 destinations = bitboards.at(i).at(1);

		for(int j = 0; j < SQUARES; j++){
			if((source & _SquareBits[ j ]) ==  _SquareBits[ j ] ){
				int x =  j & 7;
				int y =  j >> 3; 

				for(int k = 0; k < SQUARES; k++){
					if ((destinations & _SquareBits[ k ]) ==  _SquareBits[ k ] ){

						int x2 =  k & 7;
						int y2 =  k >> 3; 

						strVector.insert(strVector.end(), movesAsString(x,y,x2,y2)), pointIsSet = false;
					}
				}
			}
		}
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

