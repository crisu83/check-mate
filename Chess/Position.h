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

	/**
		functions for move generation
	*/

	//for pawns
	UI64 wSinglepushtargets(UI64 BitBoards[]);
	UI64 wDoublepushtargets(UI64 BitBoards[]);
	UI64 bSinglepushtargets(UI64 BitBoards[]);
	UI64 bDoublepushtargets(UI64 BitBoards[]);
	UI64 wPawnAttacks(UI64 BitBoards[]);
	UI64 wPawnDblAttacks(UI64 BitBoards[]);
	UI64 wPawnSingleAttacks(UI64 BitBoards[]);
	UI64 bPawnAttacks(UI64 BitBoards[]);
	UI64 bPawnDblAttacks(UI64 BitBoards[]);
	UI64 bPawnSingleAttacks(UI64 BitBoards[]);
	//for knights
	UI64 KnoNoEa(UI64 knight);
	UI64 KnoEaEa(UI64 knight);
	UI64 KsoEaEa(UI64 knight);
	UI64 KsoSoEa(UI64 knight);
	UI64 KnoNoWe(UI64 knight);
	UI64 KnoWeWe(UI64 knight);
	UI64 KsoWeWe(UI64 knight);
	UI64 KsoSoWe(UI64 knight);
	UI64 AllWhiteKnightMoves(UI64 BitBoards[]);
	UI64 AllBlackKnightMoves(UI64 BitBoards[]);



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
	int genLegalMoves(Move *moveList);

	void endTurn();

	std::string getTurnName() const;

	int *getMap();
};

