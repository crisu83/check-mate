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
	@param White pawns and empty squares
	@return all single push targets for white pawns
*/
UI64 Position::wSinglePushTargets(UI64 w_pawn, UI64 emptysquares){
	return (w_pawn << 8) & emptysquares;
}

/**
	Finds all double push targets for white pawns

	@author Arttu Nieminen, Olli Koskinen
	@param White pawns and empty squares
	@return all double push targets for white pawns
*/
UI64 Position::wDoublePushTargets(UI64 w_pawn, UI64 emptysquares){
	UI64 rank4 = 0x00000000FF000000; // rank 4 for so we can determine if doublepush possible
	UI64 singlepushes = wSinglePushTargets(w_pawn, emptysquares);
	return ((singlepushes << 8) & emptysquares) & rank4;
}

/**
	Finds all single push targets for black pawns

	@author Arttu Nieminen, Olli Koskinen
	@param Black pawns and empty squares
	@return all single push targets for black pawns
*/
UI64 Position::bSinglePushTargets(UI64 b_pawn, UI64 emptysquares){
	return (b_pawn >> 8) & emptysquares;
}

/**
	Finds all double push targets for black pawns

	@author Arttu Nieminen, Olli Koskinen
	@param Black pawns and empty squares
	@return all double push targets for black pawns
*/
UI64 Position::bDoublePushTargets(UI64 b_pawn, UI64 emptysquares){
	UI64 rank5 = 0xFF00000000; 
	UI64 singlepushes = bSinglePushTargets(b_pawn, emptysquares);
	return ((singlepushes >> 8) & emptysquares) & rank5;
}

/**
	White Pawn Attacks 
	
*/

/**
	Finds all possible white pawn attacks

	@author Arttu Nieminen, Olli Koskinen
	@param White pawns and black pieces
	@return all possible white pawn attacks
*/
UI64 Position::wPawnAttacks(UI64 w_pawn, UI64 b_pieces) {
   return (((w_pawn << 9) & ~A_FILE) | ((w_pawn << 7)& ~H_FILE)) & b_pieces;
}

/**
	Finds all possible white pawn double attacks

	@author Arttu Nieminen, Olli Koskinen
	@param White pawns and black pieces
	@return all possible white pawn double attacks
*/
UI64 Position::wPawnDblAttacks(UI64 w_pawn,  UI64 b_pieces) {
   return (((w_pawn << 9)& ~A_FILE) & ((w_pawn << 7)& ~A_FILE)) & b_pieces;
}

/**
	Finds all white pawn single attacks

	@author Arttu Nieminen, Olli Koskinen
	@param White pawns and black pieces
	@return all white pawn single attacks
*/
UI64 Position::wPawnSingleAttacks(UI64 w_pawn,  UI64 b_pieces) {
   return (((w_pawn << 9)& ~A_FILE) ^ ((w_pawn << 7)& ~H_FILE)) & b_pieces;
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
	@param Black pawns and white pieces
	@return all possible black pawn attacks
*/
UI64 Position::bPawnAttacks(UI64 b_pawn, UI64 w_pieces) {
   return (((b_pawn >> 7)& ~A_FILE) | ((b_pawn >> 9) & ~H_FILE)) & w_pieces;
}

/**
	Finds all possible black pawn double attacks

	@author Arttu Nieminen, Olli Koskinen
	@param Black pawns and white pieces
	@return all possible black pawn double attacks
*/
UI64 Position::bPawnDblAttacks(UI64 b_pawn, UI64 w_pieces) {
   return (((b_pawn >> 7)& ~A_FILE) & ((b_pawn >> 9)& ~H_FILE)) & w_pieces;
}

/**
	Finds all possible black pawn single attacks

	@author Arttu Nieminen, Olli Koskinen
	@param Black pawns and white pieces
	@return all possible black pawn single attacks
*/
UI64 Position::bPawnSingleAttacks(UI64 b_pawn, UI64 w_pieces) {
   return (((b_pawn >> 7)& ~A_FILE) ^ ((b_pawn >> 9)& ~H_FILE)) & w_pieces;
}

/**
	Finds all possible white pawn moves

	@author Arttu Nieminen, Olli Koskinen
	@param Black pawns and white pieces
	@return all possible white pawn moves
*/
UI64 Position::wAllPawnMoves(UI64 BitBoards[]){
	UI64 single = wSinglePushTargets(BitBoards[ W_PAWN ], BitBoards[ EMPTYSQUARES ]);
	UI64 dbl = wDoublePushTargets(BitBoards[ W_PAWN ], BitBoards[ EMPTYSQUARES ]);
	return wPawnAttacks(BitBoards[ W_PAWN ], BitBoards[ B_PIECES ]) | single | dbl;
}

/**
	Finds all possible black pawn moves

	@author Arttu Nieminen, Olli Koskinen
	@param Black pawns and white pieces
	@return all possible black pawn moves
*/
UI64 Position::bAllPawnMoves(UI64 BitBoards[]){
	UI64 single = bSinglePushTargets(BitBoards[ B_PAWN ], BitBoards[ EMPTYSQUARES ]);
	UI64 dbl = bDoublePushTargets(BitBoards[ B_PAWN ], BitBoards[ EMPTYSQUARES ]);
	return bPawnAttacks(BitBoards[ B_PAWN ], BitBoards[ W_PIECES ]) | single | dbl;
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
UI64 Position::KnoNoEa(UI64 knight){
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


UI64 Position::KnoEaEa(UI64 knight){
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

UI64 Position::KsoEaEa(UI64 knight){
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

UI64 Position::KsoSoEa(UI64 knight){
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

UI64 Position::KnoNoWe(UI64 knight){
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

UI64 Position::KnoWeWe(UI64 knight){
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
UI64 Position::KsoWeWe(UI64 knight){
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
UI64 Position::KsoSoWe(UI64 knight) {
	return (knight >> 17) & ~H_FILE;
}

/**
	All White Knight Moves
	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return White Knight Moves
*/
UI64 Position::AllWhiteKnightMoves(UI64 BitBoards[]){
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
UI64 Position::AllBlackKnightMoves(UI64 BitBoards[]){
	UI64 knight = BitBoards[ B_KNIGHT ];
	UI64 pseudolegal = (KnoNoEa(knight) | KnoEaEa(knight) | KsoEaEa( knight) | KsoSoEa(knight) | KnoNoWe(knight) | KnoWeWe(knight) | KsoWeWe(knight) | KsoSoWe(knight));
	return pseudolegal & ~BitBoards[ B_PIECES ];
}

 /**
	Finds moves for King


king					 king << 1			|    	king >> 1			==	
 0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0				0 0 0 0 0 0 0 0				0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
 0 0 0 0 1 0 0 0        0 0 0 0 0 1 0 0             0 0 0 1 0 0 0 0             0 0 0 1 0 1 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
 
 
 king					 king << 8			|    	king >> 8			==	
 0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0				0 0 0 0 0 0 0 0				0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 1 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 1 0 0 0
 0 0 0 0 1 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0             0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0        0 0 0 0 0 0 0 0             0 0 0 0 1 0 0 0             0 0 0 0 1 0 0 0
 
 (king << 1) | (king >> 1)	|	(king << 8) | (king >> 8)	==	
 0 0 0 0 0 0 0 0				0 0 0 0 0 0 0 0						0 0 0 0 0 0 0 0	
 0 0 0 0 0 0 0 0                0 0 0 0 0 0 0 0                     0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0                0 0 0 0 0 0 0 0                     0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0                0 0 0 0 0 0 0 0                     0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0                0 0 0 0 0 0 0 0                     0 0 0 0 0 0 0 0
 0 0 0 0 0 0 0 0                0 0 0 0 1 0 0 0                     0 0 0 0 1 0 0 0
 0 0 0 1 0 1 0 0                0 0 0 0 0 0 0 0                     0 0 0 1 0 1 0 0
 0 0 0 0 0 0 0 0                0 0 0 0 1 0 0 0                     0 0 0 0 1 0 0 0
 
	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of king
	@return All king moves
 */
 UI64 Position::kingMoves(UI64 king, UI64 ownpieces) {
   UI64 moves = ((king << 1)& ~A_FILE) | ((king >> 1) & ~H_FILE);
   moves  = (((king << 8) | (king >> 8)) | moves) & ~ownpieces;
   return moves;
}