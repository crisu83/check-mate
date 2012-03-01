#pragma once
#include "stdafx.h"


#define WHITE 0
#define BLACK 1

class Position
{
private:
	int _map[8][8];
	int _toMove;
	bool _whiteCastleShortAllowed;
	bool _whiteCastleLongAllowed;
	bool _blackCastleShortAllowed;
	bool _blackCastleLongAllowed;
	int _enPassantOnLine;
	std::vector<Move> legalMoves;
	std::vector<std::vector<UI64>> moveVector;

	/**
		functions for move generation
	*/

	//for pawns
	UI64 wSinglePushTargets(UI64 w_pawns, UI64 emptysquares);
	UI64 wDoublePushTargets(UI64 w_pawn, UI64 emptysquares);
	UI64 bSinglePushTargets(UI64 w_pawn, UI64 emptysquares);
	UI64 bDoublePushTargets(UI64 b_pawn, UI64 emptysquares);
	UI64 wPawnAttacks(UI64 w_pawn, UI64 b_pieces);
	UI64 wPawnDblAttacks(UI64 w_pawn,  UI64 b_pieces);
	UI64 wPawnSingleAttacks(UI64 w_pawn,  UI64 b_pieces);
	UI64 bPawnAttacks(UI64 b_pawn, UI64 w_pieces);
	UI64 bPawnDblAttacks(UI64 b_pawn, UI64 w_pieces);
	UI64 bPawnSingleAttacks(UI64 b_pawn, UI64 w_pieces);
	UI64 wAllPawnMoves(UI64 w_pawn, UI64 emptysquares, UI64 b_pieces);
	UI64 bAllPawnMoves(UI64 b_pawn, UI64 emptysquares, UI64 w_pieces);
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
	UI64 kingMoves(UI64 king, UI64 ownpieces);



public:
	Position(void);
	~Position(void);

	// TODO (1)
	//
	// Empties the board.
	//
	void clear();

	// TODO (2)
	//
	// Sets the inital chess position, 16 pieces each,
	// white to move.
	void initPos();

	// TODO (3)
	//
	// Print position in ASCII for testing purposes.
	//
	//void render() const;

	// TODO (4)
	//
	// Update position with given move. We can assume
	// that given move is legal - no need to check
	// game rules.
	//
	//void execMove(const Move *m);

	// TODO (5) - BIG ONE
	//
	// Move Generator. Generate all the legal moves
	// in current position. The moves are put in the
	// moveList. Returns # moves found. Returning 0
	// means there are no legal moves; the game has
	// ended (in either checkmate and stalemate).
	//
	std::vector<std::vector<UI64>> genLegalMoves(UI64 BitBoards[]);

	void setToMove(int i);
	int  getToMove();
	void endTurn();

	std::string getTurnName() const;

	int *getMap();
};

