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
	_toMove = WHITE;

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

Lets find the first pawn 			Get the intersection with 			And Tadaa! We have the least significant 1 bit
from pawns bitboard					its 2's complement


w_pawn					&				-w_pawn			==		 		LS1B
0 0 0 0 0 0 0 0						  1 1 1 1 1 1 1 1					0 0 0 0 0 0 0 0		
0 0 0 0 0 0 0 0                       1 1 1 1 1 1 1 1                   0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                       1 1 1 1 1 1 1 1                   0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                       1 1 1 1 1 1 1 1                   0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                       1 1 1 1 1 1 1 1                   0 0 0 0 0 0 0 0
0 0 1 0 0 0 0 0                       1 1 0 1 1 1 1 1                   0 0 0 0 0 0 0 0
1 1 0 1 0 1 1 1                       1 0 1 0 1 0 0 0                   1 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                       1 1 1 1 1 1 1 1                   0 0 0 0 0 0 0 0

Then we can get the moves for the first pawn

LS1B							b_pieces							wAllPawnMoves(LS1B, emptysquares, b_pieces)
0 0 0 0 0 0 0 0					1 1 1 1 1 1 1 1						0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                 1 1 1 1 1 1 1 1                     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                 0 0 0 0 0 0 0 0                     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                 0 0 0 0 0 0 0 0                     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                 0 0 0 0 0 0 0 0                     1 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                 0 0 0 0 0 0 0 0                     1 0 0 0 0 0 0 0
1 0 0 0 0 0 0 0                 0 0 0 0 0 0 0 0                     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                 0 0 0 0 0 0 0 0                     0 0 0 0 0 0 0 0

now we know the moves for the first pawn
LS1B -> wAllPawnMoves(LS1B, emptysquares, b_pieces)

We can reset the LS1B from w_pawns with
w_pawn &  (w_pawn-1) ==  w_pawn_with_reset_LS1B

And then get the moves for second pawn.

We can use the same logic for all pieces(except for king).

	
	@author Arttu Nieminen, Olli Koskinen
	@param moveList the list in which to store the moves
	@return the number of legal moves
*/
std::vector<std::vector<UI64>> Position::genLegalMoves(UI64 BitBoards[])
{
	moveVector.clear();

	int count = 0;

	if(this->_toMove == WHITE){
		if(wIsCheck(BitBoards) == false){
			//pawn moves
			UI64 w_pawn = BitBoards[ W_PAWN ];
			while((w_pawn & -w_pawn) != BitBoards[ EMPTY ]){ //while we have pawns to go through
				std::vector<UI64> tempMove;
				if(wIsPinned(w_pawn & -w_pawn, BitBoards) == false){
					tempMove.insert(tempMove.end(), w_pawn & -w_pawn); //LS1B
					tempMove.insert(tempMove.end(), wAllPawnMoves(w_pawn & -w_pawn, BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}
				w_pawn = w_pawn &  (w_pawn-1); //reset the LS1B so we can get the next pawn
			}
		
			//knight moves
			UI64 w_knight = BitBoards[ W_KNIGHT ];
			while((w_knight & -w_knight) != BitBoards[ EMPTY ]){ //while we have knights to go through
				std::vector<UI64> tempMove;
				if(wIsPinned(w_knight & -w_knight, BitBoards) == false){
					tempMove.insert(tempMove.end(), w_knight & -w_knight); //LS1B
					tempMove.insert(tempMove.end(), AllWhiteKnightMoves(w_knight & -w_knight, BitBoards[ W_PIECES ])); //all its moves
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}
				w_knight = w_knight &  (w_knight-1); //reset the LS1B so we can get the next knight
			}

			//rook moves
			UI64 w_rook = BitBoards[ W_ROOK ];
			while((w_rook & -w_rook) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				if(wIsPinned(w_rook & -w_rook, BitBoards) == false){
					tempMove.insert(tempMove.end(), w_rook & -w_rook);
					tempMove.insert(tempMove.end(), AllRookMoves(w_rook & -w_rook, BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}else{
					tempMove.insert(tempMove.end(), w_rook & -w_rook);
					tempMove.insert(tempMove.end(), wMovesForPinned(w_rook & -w_rook, AllRookMoves(w_rook & -w_rook, BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]), BitBoards));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}
				w_rook = w_rook & (w_rook-1);
			}

			//bishop moves
			UI64 w_bishop = BitBoards[ W_BISHOP ];
			while((w_bishop & -w_bishop) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				if(wIsPinned(w_bishop & -w_bishop, BitBoards) == false){
					tempMove.insert(tempMove.end(), w_bishop & -w_bishop);
					tempMove.insert(tempMove.end(), AllBishopMoves(w_bishop & -w_bishop, BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}else{
					tempMove.insert(tempMove.end(), w_bishop & -w_bishop);
					tempMove.insert(tempMove.end(), wMovesForPinned(w_bishop & -w_bishop, AllBishopMoves(w_bishop & -w_bishop, BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]), BitBoards));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}
				w_bishop = w_bishop & (w_bishop-1);
			}

			//queen moves
			UI64 w_queen = BitBoards[ W_QUEEN ];
			while((w_queen & -w_queen) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				if(wIsPinned(w_queen & -w_queen, BitBoards) == false){
					tempMove.insert(tempMove.end(), w_queen & -w_queen);
					tempMove.insert(tempMove.end(), queenMoves(w_queen & -w_queen, BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}else{
					tempMove.insert(tempMove.end(), w_queen & -w_queen);
					tempMove.insert(tempMove.end(), wMovesForPinned(w_queen & -w_queen, queenMoves(w_queen & -w_queen, BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]), BitBoards));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}
				w_queen = w_queen & (w_queen-1);
			}

			//king moves
			std::vector<UI64> tempMove;
			tempMove.insert(tempMove.end(), BitBoards[ W_KING ] );
			tempMove.insert(tempMove.end(),wKingMoves(BitBoards[ W_KING ], BitBoards[ W_PIECES ], BitBoards)& ~wCheckEnemyAttacks(BitBoards[ W_KING ], BitBoards));
			count++;
			moveVector.insert(moveVector.end(), tempMove);
		}else{
			//rook moves
			UI64 w_rook = BitBoards[ W_ROOK ];
			while((w_rook & -w_rook) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				tempMove.insert(tempMove.end(), w_rook & -w_rook);
				tempMove.insert(tempMove.end(), wBlockCheck(w_rook & -w_rook, AllRookMoves(w_rook & -w_rook, BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]), BitBoards));
				moveVector.insert(moveVector.end(), tempMove);
				count++;
				w_rook = w_rook & (w_rook-1);
			}
			UI64 w_bishop = BitBoards[ W_BISHOP ];
			while((w_bishop & -w_bishop) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				tempMove.insert(tempMove.end(), w_bishop & -w_bishop);
				tempMove.insert(tempMove.end(), wBlockCheck(w_bishop & -w_bishop, AllBishopMoves(w_bishop & -w_bishop, BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]), BitBoards));
				moveVector.insert(moveVector.end(), tempMove);
				count++;
				w_bishop = w_bishop & (w_bishop-1);
			}
			UI64 w_queen = BitBoards[ W_QUEEN ];
			while((w_queen & -w_queen) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				tempMove.insert(tempMove.end(), w_queen & -w_queen);
				tempMove.insert(tempMove.end(), wBlockCheck(w_queen & -w_queen, queenMoves(w_queen & -w_queen, BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]), BitBoards));
				moveVector.insert(moveVector.end(), tempMove);
				count++;
				w_queen = w_queen & (w_queen-1);
			}

			//king moves
			std::vector<UI64> tempMove;
			tempMove.insert(tempMove.end(), BitBoards[ W_KING ] );
			tempMove.insert(tempMove.end(), wEscapeMoves(BitBoards[ W_KING ], BitBoards));
			count++;
			moveVector.insert(moveVector.end(), tempMove);
		}

	}else if(_toMove == BLACK){
		if(bIsCheck(BitBoards) == false){
			//pawn moves
			UI64 b_pawn = BitBoards[ B_PAWN ];
			while((b_pawn & -b_pawn) != BitBoards[ EMPTY ]){ //while we have pawns to go through
				std::vector<UI64> tempMove;
				if(bIsPinned(b_pawn & -b_pawn, BitBoards) == false){
					tempMove.insert(tempMove.end(), b_pawn & -b_pawn); //LS1B
					tempMove.insert(tempMove.end(),bAllPawnMoves(b_pawn & -b_pawn, BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ])); //all its moves
					count++;
					b_pawn = b_pawn &  (b_pawn-1); //reset the LS1B so we can get the next pawn
					moveVector.insert(moveVector.end(), tempMove);
				}
			}

			//knight moves
			UI64 b_knight = BitBoards[ B_KNIGHT ];
			while((b_knight & -b_knight) != BitBoards[ EMPTY ]){ //while we have knights to go through
				std::vector<UI64> tempMove;
				if(bIsPinned(b_knight & -b_knight, BitBoards) == false){
					tempMove.insert(tempMove.end(), b_knight & -b_knight); //LS1B
					tempMove.insert(tempMove.end(),AllBlackKnightMoves(b_knight & -b_knight, BitBoards[ B_PIECES ])); //all its moves
					count++;
					b_knight = b_knight &  (b_knight-1); //reset the LS1B so we can get the next knight
					moveVector.insert(moveVector.end(), tempMove);
				}
			}
			//rook moves
			UI64 b_rook = BitBoards[ B_ROOK ];
			while((b_rook & -b_rook) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				if(wIsPinned(b_rook & -b_rook, BitBoards) == false){
					tempMove.insert(tempMove.end(), b_rook & -b_rook);
					tempMove.insert(tempMove.end(), AllRookMoves(b_rook & -b_rook, BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}else{
					tempMove.insert(tempMove.end(), b_rook & -b_rook);
					tempMove.insert(tempMove.end(), wMovesForPinned(b_rook & -b_rook, AllRookMoves(b_rook & -b_rook, BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]), BitBoards));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}
				b_rook = b_rook & (b_rook-1);
			}
			//bishop moves
			UI64 b_bishop = BitBoards[ B_BISHOP ];
			while((b_bishop & -b_bishop) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				if(wIsPinned(b_bishop & -b_bishop, BitBoards) == false){
					tempMove.insert(tempMove.end(), b_bishop & -b_bishop);
					tempMove.insert(tempMove.end(), AllBishopMoves(b_bishop & -b_bishop, BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}else{
					tempMove.insert(tempMove.end(), b_bishop & -b_bishop);
					tempMove.insert(tempMove.end(), wMovesForPinned(b_bishop & -b_bishop, AllRookMoves(b_bishop & -b_bishop, BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]), BitBoards));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}
				b_bishop = b_bishop & (b_bishop-1);
			}

			//queen moves
			UI64 b_queen = BitBoards[ B_QUEEN ];
			while((b_queen & -b_queen) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				if(wIsPinned(b_queen & -b_queen, BitBoards) == false){
					tempMove.insert(tempMove.end(), b_queen & -b_queen);
					tempMove.insert(tempMove.end(), queenMoves(b_queen & -b_queen, BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}else{	
					tempMove.insert(tempMove.end(), b_queen & -b_queen);
					tempMove.insert(tempMove.end(), wMovesForPinned(b_queen & -b_queen, queenMoves(b_queen & -b_queen, BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]), BitBoards));
					moveVector.insert(moveVector.end(), tempMove);
					count++;
				}
				b_queen = b_queen & (b_queen-1);
			}

			//king moves
			std::vector<UI64> tempMove;
			tempMove.insert(tempMove.end(),BitBoards[ B_KING ]);
			tempMove.insert(tempMove.end(),bKingMoves(BitBoards[ B_KING ], BitBoards[ B_PIECES ], BitBoards)& ~bCheckEnemyAttacks(BitBoards[ B_KING ], BitBoards));
			count++;
			moveVector.insert(moveVector.end(), tempMove);
		}else{
			UI64 b_rook = BitBoards[ B_ROOK ];
			while((b_rook & -b_rook) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				tempMove.insert(tempMove.end(), b_rook & -b_rook);
				tempMove.insert(tempMove.end(), bBlockCheck(b_rook & -b_rook, AllRookMoves(b_rook & -b_rook, BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]), BitBoards));
				moveVector.insert(moveVector.end(), tempMove);
				count++;
				b_rook = b_rook & (b_rook-1);
			}
			UI64 b_bishop = BitBoards[ B_BISHOP ];
			while((b_bishop & -b_bishop) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				tempMove.insert(tempMove.end(), b_bishop & -b_bishop);
				tempMove.insert(tempMove.end(), bBlockCheck(b_bishop & -b_bishop, AllBishopMoves(b_bishop & -b_bishop, BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]), BitBoards));
				moveVector.insert(moveVector.end(), tempMove);
				count++;
				b_bishop = b_bishop & (b_bishop-1);
			}
			UI64 b_queen = BitBoards[ B_QUEEN ];
			while((b_queen & -b_queen) != BitBoards[ EMPTY ]){
				std::vector<UI64> tempMove;
				tempMove.insert(tempMove.end(), b_queen & -b_queen);
				tempMove.insert(tempMove.end(), bBlockCheck(b_queen & -b_queen, queenMoves(b_queen & -b_queen, BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]), BitBoards));
				moveVector.insert(moveVector.end(), tempMove);
				count++;
				b_queen = b_queen & (b_queen-1);
			}

			//king moves
			std::vector<UI64> tempMove;
			tempMove.insert(tempMove.end(), BitBoards[ B_KING ] );
			tempMove.insert(tempMove.end(), bEscapeMoves(BitBoards[ B_KING ],BitBoards));
			count++;
			moveVector.insert(moveVector.end(), tempMove);
		}
	}
			return moveVector;
}

int *Position::getMap()
{
	return *_map;
}

void Position::setToMove(int i){
	Position::_toMove = i;
}

int Position::getToMove(){
	return _toMove;
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



 white pawns       white pawns << 9  &       notAFile     (w_pawn << 9) & ~A_FILE)	 &	black pieces	==
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0		1 1 1 1 1 1 1 1    	0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0       0 0 0 0 0 0 0 0		0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0       0 0 0 0 0 0 1 0     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 1 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 1 0 0       0 0 0 0 0 0 0 0     0 0 0 0 0 1 0 0 
0 0 0 0 1 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0       0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0       0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0       0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0      0 1 1 1 1 1 1 1      0 0 0 0 0 0 0 0       0 0 0 0 0 0 0 0     0 0 0 0 0 0 0 0


	@author Arttu Nieminen, Olli Koskinen
	@param White pawns and black pieces
	@return all possible white pawn attacks
*/
UI64 Position::wPawnAttacks(UI64 w_pawn, UI64 b_pieces) {
	return ((w_pawn << 9) & ~A_FILE) | ((w_pawn << 7) & ~H_FILE);
}

/**
	Finds all possible white pawn double attacks

	@author Arttu Nieminen, Olli Koskinen
	@param White pawns and black pieces
	@return all possible white pawn double attacks
*/
UI64 Position::wPawnDblAttacks(UI64 w_pawn,  UI64 b_pieces) {
   return (((w_pawn << 9)& ~A_FILE) & ((w_pawn << 7)& ~A_FILE));
}

/**
	Finds all white pawn single attacks

	@author Arttu Nieminen, Olli Koskinen
	@param White pawns and black pieces
	@return all white pawn single attacks
*/
UI64 Position::wPawnSingleAttacks(UI64 w_pawn,  UI64 b_pieces) {
   return (((w_pawn << 9)& ~A_FILE) ^ ((w_pawn << 7)& ~H_FILE));
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
   return ((((b_pawn >> 7)& ~A_FILE)) | ((b_pawn >> 9) & ~H_FILE));
}

/**
	Finds all possible black pawn double attacks

	@author Arttu Nieminen, Olli Koskinen
	@param Black pawns and white pieces
	@return all possible black pawn double attacks
*/
UI64 Position::bPawnDblAttacks(UI64 b_pawn, UI64 w_pieces) {
   return (((b_pawn >> 7)& ~A_FILE) & ((b_pawn >> 9)& ~H_FILE));
}

/**
	Finds all possible black pawn single attacks

	@author Arttu Nieminen, Olli Koskinen
	@param Black pawns and white pieces
	@return all possible black pawn single attacks
*/
UI64 Position::bPawnSingleAttacks(UI64 b_pawn, UI64 w_pieces) {
   return (((b_pawn >> 7)& ~A_FILE) ^ ((b_pawn >> 9)& ~H_FILE));
}

/**
	Finds all possible white pawn moves

	@author Arttu Nieminen, Olli Koskinen
	@param Black pawns and white pieces
	@return all possible white pawn moves
*/
UI64 Position::wAllPawnMoves(UI64 w_pawn, UI64 emptysquares, UI64 b_pieces){
	UI64 pushes = (wPawnAttacks(w_pawn, b_pieces) & ~b_pieces & ~emptysquares);
	pushes |= wSinglePushTargets(w_pawn, emptysquares);
	pushes |= wDoublePushTargets(w_pawn, emptysquares);
	return pushes;
}

/**
	Finds all possible black pawn moves

	@author Arttu Nieminen, Olli Koskinen
	@param Black pawns and white pieces
	@return all possible black pawn moves
*/
UI64 Position::bAllPawnMoves(UI64 b_pawn, UI64 emptysquares, UI64 w_pieces){
	UI64 pushes = (bPawnAttacks(b_pawn, w_pieces) & ~w_pieces & ~emptysquares);
	pushes |= bSinglePushTargets(b_pawn, emptysquares);
	pushes |= bDoublePushTargets(b_pawn, emptysquares);
	return pushes;
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
UI64 Position::AllWhiteKnightMoves(UI64 w_knight, UI64 w_pieces){
	UI64 knight = w_knight;
	UI64 pseudolegal = (KnoNoEa(knight) | KnoEaEa(knight) | KsoEaEa( knight) | KsoSoEa(knight) | KnoNoWe(knight) | KnoWeWe(knight) | KsoWeWe(knight) | KsoSoWe(knight));
	return pseudolegal & ~w_pieces;
}

/**
	All Black Knight Moves
	@author Arttu Nieminen, Olli Koskinen
	@param BitBoards array
	@return Black Knight Moves
*/
UI64 Position::AllBlackKnightMoves(UI64 b_knight, UI64 b_pieces){
	UI64 knight = b_knight;
	UI64 pseudolegal = (KnoNoEa(knight) | KnoEaEa(knight) | KsoEaEa( knight) | KsoSoEa(knight) | KnoNoWe(knight) | KnoWeWe(knight) | KsoWeWe(knight) | KsoSoWe(knight));
	return pseudolegal & ~b_pieces;
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
 UI64 Position::wKingMoves(UI64 king, UI64 ownpieces, UI64 BitBoards[]) {
   UI64 moves = ((king << 1)& ~A_FILE) | ((king >> 1) & ~H_FILE);//right-left
   moves  = (((king << 8) | (king >> 8)) | moves); //up-down
   moves |= ((((king >> 7)& ~A_FILE)) | ((king >> 9) & ~H_FILE));//diagonal down
   moves |= ((king << 9) & ~A_FILE) | ((king << 7) & ~H_FILE); //diagonal up
   return (moves & ~ownpieces);
}

 UI64 Position::bKingMoves(UI64 king, UI64 ownpieces, UI64 BitBoards[]) {
   UI64 moves = ((king << 1)& ~A_FILE) | ((king >> 1) & ~H_FILE);//right-left
   moves  = (((king << 8) | (king >> 8)) | moves); //up-down
   moves |= ((((king >> 7)& ~A_FILE)) | ((king >> 9) & ~H_FILE));//diagonal down
   moves |= ((king << 9) & ~A_FILE) | ((king << 7) & ~H_FILE); //diagonal up
   return (moves & ~ownpieces);
}

 /**
	Rooks East moves

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of rooks, emptysquares
	@return All rook east moves
 */
 UI64 Position::rEast(UI64 rooks, UI64 emptysquares) {
   emptysquares  = emptysquares & ~A_FILE;
   rooks |= emptysquares & (rooks << 1); 
   rooks |= emptysquares & (rooks << 1); 
   rooks |= emptysquares & (rooks << 1); 
   rooks |= emptysquares & (rooks << 1); 
   rooks |= emptysquares & (rooks << 1); 
   rooks |= emptysquares & (rooks << 1); 
   return ~A_FILE & (rooks << 1);
}

  /**
	Rooks West moves

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of rooks, emptysquares
	@return All rook West moves
 */
 UI64 Position::rWest(UI64 rooks, UI64 emptysquares) {
   emptysquares  = emptysquares & ~H_FILE;
   rooks |= emptysquares & (rooks >> 1); 
   rooks |= emptysquares & (rooks >> 1); 
   rooks |= emptysquares & (rooks >> 1); 
   rooks |= emptysquares & (rooks >> 1); 
   rooks |= emptysquares & (rooks >> 1); 
   rooks |= emptysquares & (rooks >> 1); 
   return ~H_FILE & (rooks >> 1);
}

  /**
	Rooks North moves

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of rooks, emptysquares
	@return All rook north moves
 */
 UI64 Position::rNorth(UI64 rooks, UI64 emptysquares) {
   rooks |= emptysquares & (rooks << 8); 
   rooks |= emptysquares & (rooks << 8); 
   rooks |= emptysquares & (rooks << 8); 
   rooks |= emptysquares & (rooks << 8); 
   rooks |= emptysquares & (rooks << 8); 
   rooks |= emptysquares & (rooks << 8); 
   return (rooks << 8);
}

  /**
	Rooks South moves

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of rooks, emptysquares
	@return All rook South moves
 */
 UI64 Position::rSouth(UI64 rooks, UI64 emptysquares) {
   rooks |= emptysquares & (rooks >> 8); 
   rooks |= emptysquares & (rooks >> 8); 
   rooks |= emptysquares & (rooks >> 8); 
   rooks |= emptysquares & (rooks >> 8); 
   rooks |= emptysquares & (rooks >> 8); 
   rooks |= emptysquares & (rooks >> 8); 
   return (rooks >> 8);
}

/**
	All Rook Moves

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of rooks, emptysquares, ownpieces
	@return All rook moves

*/
UI64 Position::AllRookMoves(UI64 rooks, UI64 emptysquares, UI64 ownpieces){
	return (rWest(rooks, emptysquares) | rEast(rooks, emptysquares) | rNorth(rooks, emptysquares) | rSouth(rooks, emptysquares)) & ~ownpieces;
}

/**
	Bishop North East moves

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of bishops, emptysquares
	@return All bishop north east moves
 */
 UI64 Position::bishopNorthEast(UI64 bishop, UI64 emptysquares) {
   emptysquares  = emptysquares & ~A_FILE;
   bishop |= emptysquares & (bishop << 9); 
   bishop |= emptysquares & (bishop << 9); 
   bishop |= emptysquares & (bishop << 9); 
   bishop |= emptysquares & (bishop << 9); 
   bishop |= emptysquares & (bishop << 9); 
   bishop |= emptysquares & (bishop << 9); 
   return ~A_FILE & (bishop << 9);
}

 /**
	Bishop North West moves

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of bishops, emptysquares
	@return All bishops West moves
 */
 UI64 Position::bishopNorthWest(UI64 bishop, UI64 emptysquares) {
   emptysquares  = emptysquares & ~H_FILE;
   bishop |= emptysquares & (bishop << 7); 
   bishop |= emptysquares & (bishop << 7); 
   bishop |= emptysquares & (bishop << 7); 
   bishop |= emptysquares & (bishop << 7); 
   bishop |= emptysquares & (bishop << 7); 
   bishop |= emptysquares & (bishop << 7); 
   return ~H_FILE & (bishop << 7);
}

/**
	Rooks South West moves

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of rooks, emptysquares
	@return All bishops South West moves
 */
 UI64 Position::bishopSouthWest(UI64 bishop, UI64 emptysquares) {
   emptysquares  = emptysquares & ~H_FILE;
   bishop |= emptysquares & (bishop >> 9); 
   bishop |= emptysquares & (bishop >> 9); 
   bishop |= emptysquares & (bishop >> 9); 
   bishop |= emptysquares & (bishop >> 9); 
   bishop |= emptysquares & (bishop >> 9); 
   bishop |= emptysquares & (bishop >> 9); 
   return ~H_FILE & (bishop >> 9);
}


 /**
	Rooks South East moves

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of bishops, emptysquares
	@return All bishops South East moves
 */
 UI64 Position::bishopSouthEast(UI64 bishop, UI64 emptysquares) {
   emptysquares  = emptysquares & ~H_FILE;
   bishop |= emptysquares & (bishop >> 7); 
   bishop |= emptysquares & (bishop >> 7); 
   bishop |= emptysquares & (bishop >> 7); 
   bishop |= emptysquares & (bishop >> 7); 
   bishop |= emptysquares & (bishop >> 7); 
   bishop |= emptysquares & (bishop >> 7); 
   return ~A_FILE & (bishop >> 7);
}

/**
	All Bishop Moves

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of bishops, emptysquares, ownpieces
	@return All bishops moves
 */
UI64 Position::AllBishopMoves(UI64 bishop, UI64 emptysquares, UI64 ownpieces){
	return (bishopNorthEast(bishop, emptysquares) | bishopNorthWest(bishop, emptysquares) | bishopSouthEast(bishop, emptysquares) | bishopSouthWest(bishop, emptysquares))& ~ownpieces;
}

/**
	All Queen Moves

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboard of bishops, emptysquares, ownpieces
	@return All queens moves
 */
UI64 Position::queenMoves(UI64 queen, UI64 emptysquares, UI64 ownpieces){
	return AllRookMoves(queen, emptysquares, ownpieces) | AllBishopMoves(queen, emptysquares, ownpieces);
}
/**
	Is White in Check

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboards
	@return true if White is in check
 */
bool Position::wIsCheck(UI64 BitBoards[]) {
	UI64 ownking = BitBoards[ W_KING ];
	UI64 attacks = wCheckEnemyAttacks(ownking, BitBoards);
	if((attacks & ownking) != BitBoards[ EMPTY ]){
		return true;
	}else{
		return false;
	}
}

/**
	Is Black in Check

 	@author Arttu Nieminen, Olli Koskinen
	@param Bitboards
	@return true if black is in check
 */
bool Position::bIsCheck(UI64 BitBoards[]) {
	UI64 ownking = BitBoards[ B_KING ];
	UI64 attacks = bCheckEnemyAttacks(ownking, BitBoards);
	if((attacks & ownking) != BitBoards[ EMPTY ]){
		return true;
	}else{
		return false;
	}
}


/*
	Function for getting those moves King can't take because they make check for enemy

for example
How to check that e3-e4 		if we get queen attacks we don't get the e4
is illegal in this situation	queenattacks								kingattacks (this should be illegal)
0 0 0 0 0 0 0 0					0 0 1 0 0 0 1 0								0 0 0 0 0 0 0 0	
0 0 0 0 0 0 0 0                 0 0 1 0 0 1 0 0                             0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                 1 0 1 0 1 0 0 0                             0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                 0 1 1 1 0 0 0 0                             0 0 0 0 0 0 0 0
0 0 q 0 p 0 0 0                 1 1 q 1 0 0 0 0                             0 0 0 0 1 0 0 0
0 0 0 0 K 0 0 0                 0 1 1 1 0 0 0 0                             0 0 0 0 K 0 0 0
0 0 0 0 0 0 0 0                 1 0 1 0 1 0 0 0                             0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                 0 0 1 0 0 1 0 0                             0 0 0 0 0 0 0 0

	So we need to reset the pawn in enemypawns which king can attack, so we get the queen move which makes the kingattack illegal

enemypieces to give to queenattacks(enemypieces & ~kingattacks)   queenattacks after
0 0 0 0 0 0 0 0														0 0 1 0 0 0 1 0		
0 0 0 0 0 0 0 0                                                     0 0 1 0 0 1 0 0
0 0 0 0 0 0 0 0                                                     1 0 1 0 1 0 0 0
0 0 0 0 0 0 0 0                                                     0 1 1 1 0 0 0 0
0 0 0 0 0 0 0 0                                                     1 1 0 1 1 1 1 1
0 0 0 0 0 0 0 0                                                     0 1 1 1 0 0 0 0
0 0 0 0 0 0 0 0                                                     1 0 1 0 1 0 0 0
0 0 0 0 0 0 0 0                                                     0 0 1 0 0 1 0 0


	@author Arttu Nieminen, Olli Koskinen
	@param ownpiece,Bitboards
	@return bitboard of the squares which king can't move into

*/
UI64 Position::bCheckEnemyAttacks(UI64 ownpiece, UI64 BitBoards[]){;
	UI64 attacks = wPawnAttacks(BitBoards[ W_PAWN ], BitBoards[ W_PIECES ]);
	attacks |= AllWhiteKnightMoves(BitBoards[ W_KNIGHT ], BitBoards[ W_PIECES ]);
	attacks |= AllRookMoves(BitBoards[ W_ROOK ], BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]);
	attacks |= AllBishopMoves(BitBoards[ W_BISHOP ], BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]);
	UI64 kingattacks =  bKingMoves(ownpiece, BitBoards[ B_PIECES ], BitBoards);
	attacks |= queenMoves(BitBoards[ W_QUEEN ], BitBoards[ EMPTYSQUARES ], (BitBoards[ W_PIECES ]& ~kingattacks));
	return attacks;
}
UI64 Position::wCheckEnemyAttacks(UI64 ownpiece, UI64 BitBoards[]){
	UI64 attacks = bPawnAttacks(BitBoards[ B_PAWN ], BitBoards[ B_PIECES ]);
	attacks |= AllBlackKnightMoves(BitBoards[ B_KNIGHT ], BitBoards[ B_PIECES ]);
	attacks |= AllRookMoves(BitBoards[ B_ROOK ], BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]);
	attacks |= AllBishopMoves(BitBoards[ B_BISHOP ], BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]);
	UI64 kingattacks =  wKingMoves(ownpiece, BitBoards[ W_PIECES ], BitBoards);
	attacks |= queenMoves(BitBoards[ B_QUEEN ], (BitBoards[ EMPTYSQUARES ]), (BitBoards[ B_PIECES ]& ~kingattacks));
	return attacks;
}
/**
	Function for checking if White Piece is pinned

	@author Arttu Nieminen, Olli Koskinen
	@param ownpiece,Bitboards
	@return true if piece is in pin

*/
bool Position::wIsPinned(UI64 ownpiece, UI64 BitBoards[]){
	UI64 attacks = bPawnAttacks(BitBoards[ B_PAWN ], BitBoards[ B_PIECES ]);
	attacks |= AllBlackKnightMoves(BitBoards[ B_KNIGHT ], BitBoards[ B_PIECES ]);
	attacks |= AllRookMoves(BitBoards[ B_ROOK ], BitBoards[ EMPTYSQUARES ] | ownpiece, BitBoards[ B_PIECES ]);
	attacks |= AllBishopMoves(BitBoards[ B_BISHOP ], BitBoards[ EMPTYSQUARES ] | ownpiece, BitBoards[ B_PIECES ]);
	attacks |= queenMoves(BitBoards[ B_QUEEN ], (BitBoards[ EMPTYSQUARES ] | ownpiece), BitBoards[ B_PIECES ]);
	if((attacks & BitBoards[ W_KING ]) != BitBoards[ EMPTY ]){
		return true;
	}else{
		return false;
	}
}
/**
	Function for checking if Black Piece is pinned

	
board					BishopMoves				QueenMoves			The Bishop is in absolute pin, so we can't move it
0 0 0 0 0 0 0 0			0 0 0 0 0 0 0 1			0 0 1 1 1 0 0 0	
0 0 0 q 0 0 0 0         1 0 0 0 0 0 1 0         1 1 1 q 1 1 1 1
0 0 0 0 0 0 0 0         0 1 0 0 0 1 0 0         0 0 1 1 1 0 0 0
0 0 0 0 0 0 0 0         0 0 1 0 1 0 0 0         0 1 0 1 0 1 0 0
0 0 0 B 0 0 0 0         0 0 0 B 0 0 0 0         1 0 0 1 0 0 1 0
0 0 0 K 0 0 0 0         0 0 1 0 1 0 0 0         0 0 0 0 0 0 0 1
0 0 0 0 0 0 0 0         0 1 0 0 0 1 0 0         0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0         1 0 0 0 0 0 1 0         0 0 0 0 0 0 0 0

if we check queenmoves without the bishop we would find that it's a check

queen moves without bishop (we get here				 if queenmoves without the bishop
if we get queenmoves with 							 makes check, the piece is in pin
param emptysquares & ~bishop)

0 0 1 1 1 0 0 0										
1 1 1 q 1 1 1 1
0 0 1 1 1 0 0 0
0 1 0 1 0 1 0 0
1 0 0 1 0 0 1 0
0 0 0 1 0 0 0 1
0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0

This way we can get if piece is in a pin, but not whether it's absolute or partial pin

Example of a partial pin							pseudo-legal bishop moves		legal bishop moves
Bishop can move towards queen but nowhere else		
0 0 0 0 0 0 0 0										0 0 0 0 0 0 0 0					0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                                     0 0 0 0 0 0 0 0                 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                                     0 0 0 0 0 0 0 1                 0 0 0 0 0 0 0 0
0 0 0 0 0 0 0 0                                     0 0 0 0 0 0 1 0                 0 0 0 0 0 0 0 0
0 q 0 0 0 0 0 0                                     0 1 0 0 0 1 0 0                 0 1 0 0 0 0 0 0
0 0 0 0 0 0 0 0                                     0 0 1 0 1 0 0 0                 0 0 1 0 0 0 0 0
0 0 0 B 0 0 0 0                                     0 0 0 B 0 0 0 0                 0 0 0 B 0 0 0 0
0 0 0 0 K 0 0 0                                     0 0 1 0 K 0 0 0                 0 0 0 0 K 0 0 0

let's get the moves for straight movements and for diagonal(we have to worry only for bishop & rook moves)
											
											we can then AND both with pinned pieces movements
											
North&South				Diagonal			Diagonal & Bishopmoves
0 1 0 0 0 0 0 0			0 0 0 0 0 1 0 0		0 0 0 0 0 0 0 0
0 1 0 0 0 0 0 0         0 0 0 0 1 0 0 0     0 0 0 0 0 0 0 0
0 1 0 0 0 0 0 0         0 0 0 1 0 0 0 0     0 0 0 0 0 0 0 0
0 1 0 0 0 0 0 0         1 0 1 0 0 0 0 0     0 0 0 0 0 0 0 0
0 q 0 0 0 0 0 0         0 q 0 0 0 0 0 0     0 1 0 0 0 0 0 0
0 1 0 0 0 0 0 0         1 0 1 0 0 0 0 0     0 0 1 0 0 0 0 0
0 1 0 B 0 0 0 0         0 0 0 B 0 0 0 0     0 0 0 B 0 0 0 0
0 1 0 0 K 0 0 0         0 0 0 0 K 0 0 0     0 0 0 0 K 0 0 0

For getting Moves for pinned pieces we have to go through every enemypiece individually
and check what type of attack it is, so we can know which move is legal for us.



	@author Arttu Nieminen, Olli Koskinen
	@param ownpiece,Bitboards
	@return true if piece is in pin

*/
bool Position::bIsPinned(UI64 ownpiece, UI64 BitBoards[]){
	UI64 attacks = wPawnAttacks(BitBoards[ W_PAWN ], BitBoards[ W_PIECES ]);
	attacks |= AllWhiteKnightMoves(BitBoards[ W_KNIGHT ], BitBoards[ W_PIECES ]);
	attacks |= AllRookMoves(BitBoards[ W_ROOK ], BitBoards[ EMPTYSQUARES ] | ownpiece, BitBoards[ W_PIECES ]);
	attacks |= AllBishopMoves(BitBoards[ W_BISHOP ], BitBoards[ EMPTYSQUARES ] | ownpiece, BitBoards[ W_PIECES ]);
	attacks |= queenMoves(BitBoards[ W_QUEEN ], (BitBoards[ EMPTYSQUARES ] | ownpiece), BitBoards[ W_PIECES ]);
	if((attacks & BitBoards[ W_KING ]) != BitBoards[ EMPTY ]){
		return true;
	}else{
		return false;
	}
}
/**
	Moves for a white piece that's in pin

	@author Arttu Nieminen, Olli Koskinen
	@param ownpiece, moves, Bitboards
	@return bitboard of possible moves

*/
UI64 Position::wMovesForPinned(UI64 ownpiece, UI64 moves, UI64 BitBoards[]){
		//we need to check every enemypiece individually
		UI64 attacks = 0;
		UI64 rook = BitBoards[ B_ROOK ];
		while((rook & -rook) != BitBoards[ EMPTY ]){
			if(((rNorth(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rNorth(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rSouth(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rSouth(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rWest(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rWest(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rEast(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rEast(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			rook = rook & (rook-1);
		}
		UI64 bishop = BitBoards[ B_BISHOP ];
		while((bishop & -bishop) != BitBoards[ EMPTY ]){
			if(((bishopNorthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopNorthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopNorthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopNorthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopSouthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopSouthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopSouthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopSouthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			bishop = bishop & (bishop-1);
		}

		UI64 queen = BitBoards[ B_QUEEN ];
		while((queen & -queen) != BitBoards[ EMPTY ]){
			if(((bishopNorthEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopNorthEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopNorthWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopNorthWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopSouthWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopSouthWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopSouthEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopSouthEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rNorth(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rNorth(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rSouth(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rSouth(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			queen = queen & (queen-1);
		}
		return attacks & moves;
}

/**
	Moves for a black piece that's in pin

	@author Arttu Nieminen, Olli Koskinen
	@param ownpiece, moves, Bitboards
	@return bitboard of possible moves

*/
UI64 Position::bMovesForPinned(UI64 ownpiece, UI64 moves, UI64 BitBoards[]){
		//we need to check every enemypiece individually
		UI64 attacks = 0;
		UI64 rook = BitBoards[ W_ROOK ];
		while((rook & -rook) != BitBoards[ EMPTY ]){
			if(((rNorth(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rNorth(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rSouth(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rSouth(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rWest(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rWest(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rEast(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rEast(rook & -rook, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			rook = rook & (rook-1);
		}
		UI64 bishop = BitBoards[ W_BISHOP ];
		while((bishop & -bishop) != BitBoards[ EMPTY ]){
			if(((bishopNorthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopNorthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopNorthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopNorthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopSouthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ] ) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopSouthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopSouthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopSouthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			bishop = bishop & (bishop-1);
		}

		UI64 queen = BitBoards[ W_QUEEN ];
		while((queen & -queen) != BitBoards[ EMPTY ]){
			if(((bishopNorthEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopNorthEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopNorthWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopNorthWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopSouthWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopSouthWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((bishopSouthEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopSouthEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rNorth(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rNorth(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rSouth(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rSouth(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rWest(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			if(((rEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rEast(queen & -queen, BitBoards[ EMPTYSQUARES ] | ownpiece);
			}
			queen = queen & (queen-1);
		}
		return attacks & moves;
}

UI64 Position::wEscapeMoves(UI64 king, UI64 BitBoards[]){
	UI64 attacks = bPawnAttacks(BitBoards[ B_PAWN ], BitBoards[ B_PIECES ]);
	attacks |= AllBlackKnightMoves(BitBoards[ B_KNIGHT ], BitBoards[ B_PIECES ]);
	attacks |= AllRookMoves(BitBoards[ B_ROOK ], BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]);
	attacks |= AllBishopMoves(BitBoards[ B_BISHOP ], BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES ]);
	attacks |= queenMoves(BitBoards[ B_QUEEN ], BitBoards[ EMPTYSQUARES ], BitBoards[ B_PIECES] );
	UI64 kingmoves = wKingMoves(king, BitBoards[ W_PIECES ], BitBoards);
	return kingmoves & ~attacks;
}

UI64 Position::bEscapeMoves(UI64 king, UI64 BitBoards[]){
	UI64 attacks = wPawnAttacks(BitBoards[ W_PAWN ], BitBoards[ W_PIECES ]);
	attacks |= AllWhiteKnightMoves(BitBoards[ W_KNIGHT ], BitBoards[ W_PIECES ]);
	attacks |= AllRookMoves(BitBoards[ W_ROOK ], BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]);
	attacks |= AllBishopMoves(BitBoards[ W_BISHOP ], BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES ]);
	attacks |= queenMoves(BitBoards[ W_QUEEN ], BitBoards[ EMPTYSQUARES ], BitBoards[ W_PIECES] );
	UI64 kingmoves = bKingMoves(king, BitBoards[ B_PIECES ], BitBoards);
	return kingmoves & ~attacks;
}

UI64 Position::wBlockCheck(UI64 ownpiece, UI64 moves, UI64 BitBoards[]){
		//we need to check every enemypiece individually
		UI64 attacks = 0;
		UI64 pawn = BitBoards[ B_PAWN ];
		while((pawn & -pawn) != BitBoards[ EMPTY ]){ //while we have pawns to go through
			if(((bPawnAttacks(pawn & -pawn, BitBoards[ W_PIECES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= pawn & -pawn;
			}
			pawn = pawn &  (pawn-1); //reset the LS1B so we can get the next pawn
		}
		UI64 rook = BitBoards[ B_ROOK ];
		while((rook & -rook) != BitBoards[ EMPTY ]){
			if(((rNorth(rook & -rook, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rNorth(rook & -rook, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rSouth(rook & -rook, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rSouth(rook & -rook, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rWest(rook & -rook, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rWest(rook & -rook, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rEast(rook & -rook, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rEast(rook & -rook, BitBoards[ EMPTYSQUARES ]);
			}
			rook = rook & (rook-1);
		}
		UI64 bishop = BitBoards[ B_BISHOP ];
		while((bishop & -bishop) != BitBoards[ EMPTY ]){
			if(((bishopNorthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopNorthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopNorthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopNorthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopSouthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopSouthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopSouthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopSouthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ]);
			}
			bishop = bishop & (bishop-1);
		}

		UI64 queen = BitBoards[ B_QUEEN ];
		while((queen & -queen) != BitBoards[ EMPTY ]){
			if(((bishopNorthEast(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopNorthEast(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopNorthWest(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopNorthWest(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopSouthWest(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopSouthWest(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopSouthEast(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopSouthEast(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rNorth(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rNorth(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rSouth(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rSouth(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rWest(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rWest(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rEast(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rEast(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			queen = queen & (queen-1);
		}
		UI64 knight = BitBoards[ B_KNIGHT ];
		while((knight & -knight) != BitBoards[ EMPTY ]){ //while we have pawns to go through
			if(((AllBlackKnightMoves(knight & -knight, BitBoards[ B_PIECES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= knight & -knight;
			}
			knight = knight &  (knight-1); //reset the LS1B so we can get the next pawn
		}

		return attacks & moves;
}


UI64 Position::bBlockCheck(UI64 ownpiece, UI64 moves, UI64 BitBoards[]){
		//we need to check every enemypiece individually
		UI64 attacks = 0;
		UI64 pawn = BitBoards[ W_PAWN ];
		while((pawn & -pawn) != BitBoards[ EMPTY ]){ //while we have pawns to go through
			if(((wPawnAttacks(pawn & -pawn, BitBoards[ B_PIECES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= pawn & -pawn;
			}
			pawn = pawn &  (pawn-1); //reset the LS1B so we can get the next pawn
		}
		UI64 rook = BitBoards[ W_ROOK ];
		while((rook & -rook) != BitBoards[ EMPTY ]){
			if(((rNorth(rook & -rook, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rNorth(rook & -rook, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rSouth(rook & -rook, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rSouth(rook & -rook, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rWest(rook & -rook, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rWest(rook & -rook, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rEast(rook & -rook, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= rook & -rook;
				attacks |= rEast(rook & -rook, BitBoards[ EMPTYSQUARES ]);
			}
			rook = rook & (rook-1);
		}
		UI64 bishop = BitBoards[ W_BISHOP ];
		while((bishop & -bishop) != BitBoards[ EMPTY ]){
			if(((bishopNorthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopNorthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopNorthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopNorthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopSouthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopSouthWest(bishop & -bishop, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopSouthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= bishop & -bishop;
				attacks |= bishopSouthEast(bishop & -bishop, BitBoards[ EMPTYSQUARES ]);
			}
			bishop = bishop & (bishop-1);
		}

		UI64 queen = BitBoards[ W_QUEEN ];
		while((queen & -queen) != BitBoards[ EMPTY ]){
			if(((bishopNorthEast(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopNorthEast(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopNorthWest(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopNorthWest(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopSouthWest(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopSouthWest(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((bishopSouthEast(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= bishopSouthEast(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rNorth(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rNorth(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rSouth(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rSouth(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rWest(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rWest(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			if(((rEast(queen & -queen, BitBoards[ EMPTYSQUARES ]) & BitBoards[ B_KING ]) != BitBoards[ EMPTY ])){
				attacks |= queen & -queen;
				attacks |= rEast(queen & -queen, BitBoards[ EMPTYSQUARES ]);
			}
			queen = queen & (queen-1);
		}
		UI64 knight = BitBoards[ W_KNIGHT ];
		while((knight & -knight) != BitBoards[ EMPTY ]){ //while we have pawns to go through
			if(((AllWhiteKnightMoves(knight & -knight, BitBoards[ W_PIECES ]) & BitBoards[ W_KING ]) != BitBoards[ EMPTY ])){
				attacks |= knight & -knight;
			}
			knight = knight &  (knight-1); //reset the LS1B so we can get the next pawn
		}

		return attacks & moves;
}