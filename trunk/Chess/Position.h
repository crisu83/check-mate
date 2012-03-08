#pragma once
#include "stdafx.h"


#define WHITE 0
#define BLACK 1


class Position
{
private:
	int _map[8][8];
	int _toMove;
	int _enPassantOnLine;
	std::vector<Move> legalMoves;
	std::vector<std::vector<UI64>> moveVector;
	
	bool _whiteCastleShortAllowed;
	bool _whiteCastleLongAllowed;
	bool _blackCastleShortAllowed;
	bool _blackCastleLongAllowed;

	/**
		functions for move generation
	*/

	//for pawns
	UI64 wSinglePushTargets(UI64 w_pawns, UI64 emptysquares);
	UI64 wDoublePushTargets(UI64 w_pawn, UI64 emptysquares);
	UI64 bSinglePushTargets(UI64 w_pawn, UI64 emptysquares);
	UI64 bDoublePushTargets(UI64 b_pawn, UI64 emptysquares);
	UI64 wPawnAttacks(UI64 w_pawn);
	UI64 wPawnDblAttacks(UI64 w_pawn);
	UI64 wPawnSingleAttacks(UI64 w_pawn);
	UI64 bPawnAttacks(UI64 b_pawn);
	UI64 bPawnDblAttacks(UI64 b_pawn);
	UI64 bPawnSingleAttacks(UI64 b_pawn);
	UI64 wAllPawnMoves(UI64 w_pawn, UI64 emptysquares, UI64 BitBoards[]);
	UI64 bAllPawnMoves(UI64 b_pawn, UI64 emptysquares, UI64 BitBoards[]);
	//for knights
	UI64 KnoNoEa(UI64 knight);
	UI64 KnoEaEa(UI64 knight);
	UI64 KsoEaEa(UI64 knight);
	UI64 KsoSoEa(UI64 knight);
	UI64 KnoNoWe(UI64 knight);
	UI64 KnoWeWe(UI64 knight);
	UI64 KsoWeWe(UI64 knight);
	UI64 KsoSoWe(UI64 knight);
	UI64 AllWhiteKnightMoves(UI64 w_knight, UI64 w_pieces);
	UI64 AllBlackKnightMoves(UI64 b_knight, UI64 b_pieces);
	//king
	UI64 wKingMoves(UI64 king, UI64 ownpieces, UI64 BitBoards[]);
	UI64 bKingMoves(UI64 king, UI64 ownpieces, UI64 BitBoards[]);
	UI64 bCheckEnemyAttacks(UI64 ownpiece, UI64 BitBoards[]); //to get the squares into whick king can't move because of enemy check
	UI64 wCheckEnemyAttacks(UI64 ownpiece, UI64 BitBoards[]);
	//rook
	UI64 AllRookMoves(UI64 rooks, UI64 emptysquares, UI64 ownpieces);
	UI64 rEast(UI64 rooks, UI64 emptysquares);
	UI64 rWest(UI64 rooks, UI64 emptysquares);
	UI64 rNorth(UI64 rooks, UI64 emptysquares);
	UI64 rSouth(UI64 rooks, UI64 emptysquares);
	//bishop
	UI64 bishopNorthEast(UI64 bishop, UI64 emptysquares);
	UI64 bishopNorthWest(UI64 bishop, UI64 emptysquares);
	UI64 bishopSouthEast(UI64 bishop, UI64 emptysquares);
	UI64 bishopSouthWest(UI64 bishop, UI64 emptysquares);
	UI64 AllBishopMoves(UI64 bishop, UI64 emptysquares, UI64 ownpieces);
	//queen
	UI64 queenMoves(UI64 queen, UI64 emptysquares, UI64 ownpieces);

	//for escaping
	UI64 wEscapeMoves(UI64 king, UI64 BitBoards[]);
	UI64 bEscapeMoves(UI64 king, UI64 BitBoards[]);
	UI64 queenMovesForEscaping(UI64 queen, UI64 emptysquares);
	UI64 AllBishopMovesForEscaping(UI64 bishop, UI64 emptysquares);
	UI64 AllRookMovesForEscaping(UI64 rooks, UI64 emptysquares);
	UI64 AllWhiteKnightMovesForEscaping(UI64 w_knight);
	UI64 AllBlackKnightMovesForEscaping(UI64 b_knight);
	//isCheck-functions
	bool wIsPinned(UI64 ownpiece, UI64 BitBoards[]);
	bool bIsPinned(UI64 ownpiece, UI64 BitBoards[]);
	UI64 wMovesForPinned(UI64 ownpiece, UI64 moves, UI64 BitBoards[]);
	UI64 bMovesForPinned(UI64 ownpiece, UI64 moves, UI64 BitBoards[]);
	UI64 wBlockCheck(UI64 ownpiece, UI64 moves, UI64 BitBoards[]);
	UI64 bBlockCheck(UI64 ownpiece, UI64 moves, UI64 BitBoards[]);

	//Castling
	UI64 wAllEnemyAttacks(UI64 BitBoards[]);
	UI64 bAllEnemyAttacks(UI64 BitBoards[]);

public:
	Position(void);
	~Position(void);

	void clear();
	void initPos();

	std::vector<std::vector<UI64>> genLegalMoves(UI64 BitBoards[]);
	void setToMove(int i);
	int  getToMove();
	void endTurn();

	std::string getTurnName() const;
	int *getMap();

	//Castling
	void setWhiteCastlingFalse();
	void setBlackCastlingFalse();
	void wShortCastleFalse();
	void wLongCastleFalse();
	void bShortCastleFalse();
	void bLongCastleFalse();
	//check
	bool wIsCheck(UI64 BitBoards[]);
	bool bIsCheck(UI64 BitBoards[]);
};

