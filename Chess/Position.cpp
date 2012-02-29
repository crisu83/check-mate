#include "StdAfx.h"
#include "Position.h"

/**
	Constructor.

	@author Christoffer Niska
 */
Position::Position(void)
{
	// Castling is allowed until king or rook is moved.
	_whiteCastleShortAllowed = true;
	_whiteCastleLongAllowed = true;
	_blackCastleShortAllowed = true;
	_blackCastleLongAllowed = true;

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
	int x, y;

	for (x = 0; x < 8; x++)
	{
		for (y = 0; y < 8; y++)
		{
			_map[x][y] = EMPTY;
		}
	}
}

/**
	Sets the initial positions for the pieces on the board.

	@author Christoffer Niska
	@return void
 */
void Position::initPos()
{
	int x;

	// Paws
	for (x = 0; x < 8; x++)
	{
		_map[x][1] = B_PAWN;
		_map[x][6] = W_PAWN;
	}

	// Rooks
	_map[0][0] = B_ROOK;
	_map[7][0] = B_ROOK;
	_map[0][7] = W_ROOK;
	_map[7][7] = W_ROOK;

	// Knights
	_map[1][0] = B_KNIGHT;
	_map[6][0] = B_KNIGHT;
	_map[1][7] = W_KNIGHT;
	_map[6][7] = W_KNIGHT;

	// Bishops
	_map[2][0] = B_BISHOP;
	_map[5][0] = B_BISHOP;
	_map[2][7] = W_BISHOP;
	_map[5][7] = W_BISHOP;

	// Queens
	_map[3][0] = B_QUEEN;
	_map[3][7] = W_QUEEN;

	// Kings
	_map[4][0] = B_KING;
	_map[4][7] = W_KING;
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

int *Position::getMap()
{
	return *_map;
}



/****
	FUNCTIONS FOR MOVE GENERATION
	


	
*/

/***
	
	PAWN MOVES


*/
/**
	Finds all single push targets for white pawns

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return all single push targets for white pawns
*/
UI64 wSinglePushTargets(UI64 BitBoards[]){
	return (BitBoards[W_PAWN] << 8) & BitBoards[ EMPTYSQUARES ];
}

/**
	Finds all double push targets for white pawns

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return all double push targets for white pawns
*/
UI64 wDoublepushtargets(UI64 BitBoards[]){
	UI64 rank4 = 0x00000000FF000000; // rank 4 for so we can determine if doublepush possible
	UI64 singlepushes = (wSinglePushTargets(BitBoards + W_PAWN ) & BitBoards[ EMPTYSQUARES ]);
	return (singlepushes << 8) & BitBoards[ EMPTYSQUARES ] & rank4;
}

/**
	Finds all single push targets for black pawns

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return all single push targets for black pawns
*/
UI64 bSinglePushTargets(UI64 BitBoards[]){
	return (BitBoards[ B_PAWN ] >> 8) & BitBoards[ EMPTYSQUARES ];
}

/**
	Finds all double push targets for black pawns

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return all double push targets for black pawns
*/
UI64 bDoublepushtargets(UI64 BitBoards[]){
	UI64 rank5 = 0xFF00000000; 
	UI64 singlepushes = (bSinglePushTargets(BitBoards + B_PAWN )) & BitBoards[ EMPTYSQUARES ];
	return (singlepushes >> 8) & BitBoards[ EMPTYSQUARES ] & rank5;
}

/**
	White Pawn Attacks 
	
*/

/**
	Finds all possible white pawn attacks

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return all possible white pawn attacks
*/
UI64 wPawnAttacks(UI64 BitBoards[]) {
   return (((BitBoards[ W_PAWN ] << 9) & ~A_FILE) | ((BitBoards[ W_PAWN ] << 7)& ~H_FILE)) & BitBoards[ B_PIECES ];
}

/**
	Finds all possible white pawn double attacks

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return all possible white pawn double attacks
*/
UI64 wPawnDblAttacks(UI64 BitBoards[]) {
   return (((BitBoards[ W_PAWN ] << 9)& ~A_FILE) & ((BitBoards[ W_PAWN ] << 7)& ~A_FILE)) & BitBoards[ B_PIECES ];
}

/**
	Finds all white pawn single attacks

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return all white pawn single attacks
*/
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

/**
	Finds all possible black pawn attacks

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return all possible black pawn attacks
*/
UI64 bPawnAttacks(UI64 BitBoards[]) {
   return (((BitBoards[ B_PAWN ] >> 7)& ~A_FILE) | ((BitBoards[ B_PAWN ] >> 9) & ~H_FILE)) & BitBoards[ W_PIECES ];
}

/**
	Finds all possible black pawn double attacks

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return all possible black pawn double attacks
*/
UI64 bPawnDblAttacks(UI64 BitBoards[]) {
   return (((BitBoards[ B_PAWN ] >> 7)& ~A_FILE) & ((BitBoards[ B_PAWN ] >> 9)& ~H_FILE)) & BitBoards[ W_PIECES ];
}

/**
	Finds all possible black pawn single attacks

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return all possible black pawn single attacks
*/
UI64 bPawnSingleAttacks(UI64 BitBoards[]) {
   return (((BitBoards[ B_PAWN ] >> 7)& ~A_FILE) ^ ((BitBoards[ B_PAWN ] >> 9)& ~H_FILE)) & BitBoards[ W_PIECES ];
}

/**
	MOVES FOR KNIGHTS

*/


/**
	Knight North-North-East Moves
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
	
	@author Arttu Nieminen, Olli Koskinen
	@param BitBoard of knights
	@return all Knight North-North-East Moves
*/
UI64 KnoNoEa(UI64 knight){
	return (knight << 17) & ~A_FILE;
}

/**
	Knight North-East-East Moves
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

knights				knights << 10    & 	~(A_FILE | B_FILE)		==	
0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0			0 0 1 1 1 1 1 1			0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 1 0 0 1 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 1 0 0 0
0 0 1 0 0 0 0 1     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoard of knights
	@return all Knight North-East-East Moves
*/


UI64 KnoEaEa(UI64 knight){
	return (knight << 10) & ~(A_FILE | B_FILE);
}


/**
	Knight South-East-East Moves
	KsoEaEa



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

knights				knights >> 6    & 	~(A_FILE | B_FILE)		==	
0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0			0 0 1 1 1 1 1 1			0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0
0 0 1 0 0 0 0 1     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 1 0 0 1 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 1 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0         0 0 1 1 1 1 1 1         0 0 0 0 0 0 0 0

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoard of knights
	@return all Knight South-East-East Moves
*/

UI64 KsoEaEa(UI64 knight){
	return (knight >> 6) & ~(A_FILE | B_FILE);
}


/**
	Knight South-South-East Moves
	KnsoSoEa

knights				knights >> 15   & 		~A_FILE		 ==	
0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0		0 1 1 1 1 1 1 1		0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0
0 0 1 0 0 0 0 1     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     1 0 0 1 0 0 0 0     0 1 1 1 1 1 1 1     0 0 0 1 0 0 0 0

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoard of knights
	@return all Knight South-South-East Moves
*/

UI64 KsoSoEa(UI64 knight){
	return (knight >> 15) & ~A_FILE;
}


/**
	Knight North-North-West Moves
	KnoNoWe

knights				knights << 15   & 		~H_FILE		 ==	
0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0		1 1 1 1 1 1 1 0 	 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 1 0 0 0 0 0 1     1 1 1 1 1 1 1 0      0 1 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0
1 0 1 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoard of knights
	@return all Knight North-North-West Moves
*/

UI64 KnoNoWe(UI64 knight){
	return (knight << 15) & ~H_FILE;
}

/**
	Knight North-West-West Moves
	KnoWeWe

~G_FILE		 &		~H_FILE	 		 ==
1 1 1 1 1 1 0 1		1 1 1 1 1 1 1 0		1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0

Because of Mr. DeMorgan it's same as ~(G_FILE | H_FILE).

knights				knights << 6   & 	~(G_FILE | H_FILE) ==	
0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0		1 1 1 1 1 1 0 0    	 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     1 0 0 0 0 0 1 0     1 1 1 1 1 1 0 0      1 0 0 0 0 0 0 0
1 0 1 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoard of knights
	@return all Knight North-West-West Moves
*/

UI64 KnoWeWe(UI64 knight){
	return (knight << 6) & ~(G_FILE | H_FILE);
}

/**
	Knight South-West-West Moves
	KsoWeWe

~G_FILE		 &		~H_FILE	 		 ==
1 1 1 1 1 1 0 1		1 1 1 1 1 1 1 0		1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0
1 1 1 1 1 1 0 1     1 1 1 1 1 1 1 0     1 1 1 1 1 1 0 0

Because of Mr. DeMorgan it's same as ~(G_FILE | H_FILE).

knights				knights >> 10    & 	~(G_FILE | H_FILE) ==	
0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0		1 1 1 1 1 1 0 0    	 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0
1 0 1 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     1 0 0 0 0 0 1 0     1 1 1 1 1 1 0 0      1 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 0 0      0 0 0 0 0 0 0 0

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoard of knights
	@return all Knight South-West-West Moves
*/
UI64 KsoWeWe(UI64 knight){
	return (knight >> 10) & ~(G_FILE | H_FILE);
}


/**
	Knight South-South-West Moves
	KsoSoWe

knights				knights >> 17   & 		~H_FILE		 ==	
0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0		1 1 1 1 1 1 1 0 	 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0
1 0 1 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0     1 1 1 1 1 1 1 0      0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 1 0 0 0 0 0 1     1 1 1 1 1 1 1 0      0 1 0 0 0 0 0 0

	@author Arttu Nieminen, Olli Koskinen
	@param BitBoard of knights
	@return all Knight South-South-West Moves
*/
UI64 KsoSoWe(UI64 knight) {
	return (knight >> 17) & ~H_FILE;
}

/**
	All White Knight Moves
	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return White Knight Moves
*/
UI64 AllWhiteKnightMoves(UI64 BitBoards[]){
	UI64 knight = BitBoards[ W_KNIGHT ];
	UI64 pseudolegal = (KnoNoEa(knight) | KnoEaEa(knight) | KsoEaEa( knight) | KsoSoEa(knight) | KnoNoWe(knight) | KnoWeWe(knight) | KsoWeWe(knight) | KsoSoWe(knight));
	return pseudolegal & ~BitBoards[ W_PIECES ];
}

/**
	All Black Knight Moves
	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return Black Knight Moves
*/
UI64 AllBlackKnightMoves(UI64 BitBoards[]){
	UI64 knight = BitBoards[ B_KNIGHT ];
	UI64 pseudolegal = (KnoNoEa(knight) | KnoEaEa(knight) | KsoEaEa( knight) | KsoSoEa(knight) | KnoNoWe(knight) | KnoWeWe(knight) | KsoWeWe(knight) | KsoSoWe(knight));
	return pseudolegal & ~BitBoards[ B_PIECES ];
}