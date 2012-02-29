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
	_BitBoards[ W_PIECES]	= 0xffff;
	_BitBoards[ B_PIECES]	= 0xffff000000000000;
	_BitBoards[EMPTYSQUARES]= ~(_BitBoards[ W_PIECES ] | _BitBoards[ B_PIECES ]);
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

	// Paws
	for (x = 0; x < 8; x++)
	{
		setPieceAt(x, 1, new Piece(B_PAWN));
		setPieceAt(x, 6, new Piece(W_PAWN));
	}

	// Rooks
	setPieceAt(0, 0, new Piece(B_ROOK));
	setPieceAt(7, 0, new Piece(B_ROOK));
	setPieceAt(0, 7, new Piece(W_ROOK));
	setPieceAt(7, 7, new Piece(W_ROOK));

	// Knights
	setPieceAt(1, 0, new Piece(B_KNIGHT));
	setPieceAt(6, 0, new Piece(B_KNIGHT));
	setPieceAt(1, 7, new Piece(W_KNIGHT));
	setPieceAt(6, 7, new Piece(W_KNIGHT));

	// Bishops
	setPieceAt(2, 0, new Piece(B_BISHOP));
	setPieceAt(5, 0, new Piece(B_BISHOP));
	setPieceAt(2, 7, new Piece(W_BISHOP));
	setPieceAt(5, 7, new Piece(W_BISHOP));

	// Queens
	setPieceAt(3, 0, new Piece(B_QUEEN));
	setPieceAt(3, 7, new Piece(W_QUEEN));

	// Kings
	setPieceAt(4, 0, new Piece(B_KING));
	setPieceAt(4, 7, new Piece(W_KING));
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
			square = _squares[x][y];

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
	Updates the move to bitboards

	@author Olli Koskinen, Arttu Nieminen
	@return void
*/

void Board::updateBitBoards(Move move, int type){
	//Debug info: 
	if(debug)
		std::cout<<"Bitboards [ type ] = "<<_BitBoards[ type ]<<"\n"; 

	//Fixme
	int sourceindex = move.getX1()+ (move.getY1() * 8);
	int destindex = move.getX2() + (move.getY2() * 8);
	_BitBoards[ type ]   -=   _SquareBits[sourceindex];
	_BitBoards[ type ]	 +=   _SquareBits[destindex];

	if(debug)
		std::cout<<"Bitboards [ type ] = "<<_BitBoards[ type ]<<"\n"; 
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
bool Board::moveIsLegal(Move _curMove){

	if(getPieceAt(_curMove.getX1(),_curMove.getY1())-> getType() == 0){
		return false;
	}

	//Check legal moves plx

	updateBitBoards(_curMove, getPieceAt(_curMove.getX1(),_curMove.getY1())-> getType());
	return true;
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
	Returns the piece at the given coordinates.

	@author Christoffer Niska, Olli Koskinen, Arttu Nieminen
	@param x the x-coordinate
	@param y the y-coordinate
	@return integer representing the piece
*//*
int Board::getPieceTypeAt(int x, int y)
{
	Square *square;
	
	square = getSquareAt(x, y);

	if (square != NULL)
	{
		return square->getPiece()->getType();
	}
	return 0;
	// todo: free mem.
}*/



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


/****
	FUNCTIONS FOR MOVE GENERATION
	


	
*/

/***
	
	PAWN MOVES


*/

// Returns all single push targets for white pawns
UI64 wSinglePushTargets(UI64 BitBoards[]){
	return (BitBoards[W_PAWN] << 8) & BitBoards[ EMPTYSQUARES ];
}

// Returns all double push targets for white pawns
UI64 wDoublepushtargets(UI64 BitBoards[]){
	UI64 rank4 = 0x00000000FF000000; // rank 4 for so we can determine if doublepush possible
	UI64 singlepushes = (wSinglePushTargets(BitBoards + W_PAWN ) & BitBoards[ EMPTYSQUARES ]);
	return (singlepushes << 8) & BitBoards[ EMPTYSQUARES ] & rank4;
}

// Returns all single push targets for black pawns
UI64 bSinglePushTargets(UI64 BitBoards[]){
	return (BitBoards[ B_PAWN ] >> 8) & BitBoards[ EMPTYSQUARES ];
}

// Returns all double push targets for black pawns
UI64 bDoublepushtargets(UI64 BitBoards[]){
	UI64 rank5 = 0xFF00000000; 
	UI64 singlepushes = (bSinglePushTargets(BitBoards + B_PAWN )) & BitBoards[ EMPTYSQUARES ];
	return (singlepushes >> 8) & BitBoards[ EMPTYSQUARES ] & rank5;
}

/**
	White Pawn Attacks 
	
*/

// Returns all possible white pawn attacks
UI64 wPawnAttacks(UI64 BitBoards[]) {
   return (((BitBoards[ W_PAWN ] << 9) & ~A_FILE) | ((BitBoards[ W_PAWN ] << 7)& ~H_FILE)) & BitBoards[ B_PIECES ];
}

// Returns all possible white pawn double attacks
UI64 wPawnDblAttacks(UI64 BitBoards[]) {
   return (((BitBoards[ W_PAWN ] << 9)& ~A_FILE) & ((BitBoards[ W_PAWN ] << 7)& ~A_FILE)) & BitBoards[ B_PIECES ];
}

// Returns all white pawn single attacks
UI64 wPawnSingleAttacks(UI64 BitBoards[]) {
   return (((BitBoards[ W_PAWN ] << 9)& ~A_FILE) ^ ((BitBoards[ W_PAWN ] << 7)& ~H_FILE)) & BitBoards[ B_PIECES ];
}

/** wPawnEastAttacks

 white pawns       white pawns << 9  &       notAFile     ==   wPawnEastAttacks	 &	black pieces	==	east captures
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0		1 1 1 1 1 1 1 1    	0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0       0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0       0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0       0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0 
0 0 0 0 0 0 0 0     1 0 0 1 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 1 0 0 0 0       0 0 0 1 0 0 0 0     0 0 0 1 0 0 0 0
0 0 1 0 0 0 0 0     0 1 1 0 1 0 1 1      0 1 1 1 1 1 1 1      0 1 1 0 1 0 1 1       0 0 0 0 0 0 1 0     0 0 0 0 0 0 1 0
1 1 0 1 0 1 1 1     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0       0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0       0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0


	wPawnWestAttacks same as the east with << 7 and notHFile
	wPawnAttacks is these two combined

*/
/**
	Black Pawn Attacks 
	
*/
// Returns all possible black pawn attacks
UI64 bPawnAttacks(UI64 BitBoards[]) {
   return (((BitBoards[ B_PAWN ] >> 7)& ~A_FILE) | ((BitBoards[ B_PAWN ] >> 9) & ~H_FILE)) & BitBoards[ W_PIECES ];
}

// Returns all possible black pawn double attacks
UI64 bPawnDblAttacks(UI64 BitBoards[]) {
   return (((BitBoards[ B_PAWN ] >> 7)& ~A_FILE) & ((BitBoards[ B_PAWN ] >> 9)& ~H_FILE)) & BitBoards[ W_PIECES ];
}

// Returns all possible black pawn single attacks
UI64 bPawnSingleAttacks(UI64 BitBoards[]) {
   return (((BitBoards[ B_PAWN ] >> 7)& ~A_FILE) ^ ((BitBoards[ B_PAWN ] >> 9)& ~H_FILE)) & BitBoards[ W_PIECES ];
}

/**
	MOVES FOR KNIGHTS

*/


/**
	Knight moves

*/

// whites

/**
White Knight North-North-East
wKnoNoEa

White knights		wknights << 17   & 		~A_FILE		 ==	
0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0		0 1 1 1 1 1 1 1		0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     1 0 0 1 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 1 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0
0 0 1 0 0 0 0 1     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0

same principle for every other movement too
*/
UI64 KnoNoEa(UI64 knight){
	return (knight << 17) & ~A_FILE;
}

/**
Knight North-East-East
KnoEaEa



~A_FILE		 &		~B_FILE	 		 ==
0 1 1 1 1 1 1 1		1 0 1 1 1 1 1 1		0 0 1 1 1 1 1 1
0 1 1 1 1 1 1 1     1 0 1 1 1 1 1 1     0 0 1 1 1 1 1 1
0 1 1 1 1 1 1 1     1 0 1 1 1 1 1 1     0 0 1 1 1 1 1 1
0 1 1 1 1 1 1 1     1 0 1 1 1 1 1 1     0 0 1 1 1 1 1 1
0 1 1 1 1 1 1 1     1 0 1 1 1 1 1 1     0 0 1 1 1 1 1 1
0 1 1 1 1 1 1 1     1 0 1 1 1 1 1 1     0 0 1 1 1 1 1 1
0 1 1 1 1 1 1 1     1 0 1 1 1 1 1 1     0 0 1 1 1 1 1 1
0 1 1 1 1 1 1 1     1 0 1 1 1 1 1 1     0 0 1 1 1 1 1 1

Because of Mr. DeMorgan it's same as ~(A_FILE | B_FILE).

knights				knights << 10    & 	~(A_FILE | B_FILE)
0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0			0 0 1 1 1 1 1 1
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1
0 0 0 0 0 0 0 0     0 1 0 0 1 0 0 0         0 0 1 1 1 1 1 1
0 0 1 0 0 0 0 1     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1


*/


UI64 KnoEaEa(UI64 knight){
	return (knight << 10) & ~(A_FILE | B_FILE);
}

UI64 KsoEaEa(UI64 knight){
	return (knight >> 6) & ~(A_FILE | B_FILE);
}

UI64 KsoSoEa(UI64 knight){
	return (knight >> 17) & ~A_FILE;
}

UI64 KnoNoWe(UI64 knight){
	return (knight << 15) & ~H_FILE;
}

UI64 KnoWeWe(UI64 knight){
	return (knight << 6) & ~(G_FILE | H_FILE);
}

UI64 KsoWeWe(UI64 knight){
	return (knight >> 10) & ~(G_FILE | H_FILE);
}

UI64 KsoSoWe(UI64 knight) {
	return (knight >> 17) & ~H_FILE;
}
