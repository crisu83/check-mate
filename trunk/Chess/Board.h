#pragma once

#include "Move.h"
#include "Piece.h"

#include <iostream>

#define WHITE 0
#define BLACK 1

// Piece types
enum {
	EMPTY = 0,
	W_KING, W_QUEEN, W_ROOK, W_BISHOP, W_KNIGHT, W_PAWN,
	B_KING, B_QUEEN, B_ROOK, B_BISHOP, B_KNIGHT, B_PAWN
};

class Board
{
private:
	int _board[8][8];
	int _toMove;
	bool _whiteCastleShortAllowed;
	bool _whiteCastleLongAllowed;
	bool _blackCastleShortAllowed;
	bool _blackCastleLongAllowed;
	int _enPassantOnLine;

public:
	Board(void);
	~Board(void);

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
	void print() const;

	// TODO (4)
	//
	// Update position with given move. We can assume
	// that given move is legal - no need to check
	// game rules.
	//
	void execMove(const Move *m);

	// TODO (5) - BIG ONE
	//
	// Move Generator. Generate all the legal moves
	// in current position. The moves are put in the
	// moveList. Returns # moves found. Returning 0
	// means there are no legal moves; the game has
	// ended (in either checkmate and stalemate).
	//
	int genLegalMoves(Move *moveList);

	Piece *getPieceAt(int x, int y);
};

