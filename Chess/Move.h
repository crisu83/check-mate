#pragma once

// Max. number of legal moves in any position
#define MAX_MOVES 256

// Letters used for moves
#define LETTERS "abcdefgh"

// For promotions
enum {
	QUEEN, ROOK, BISHOP, KNIGHT
};

class Move
{
private:
	int _x1, _y1;		// start square indexes
	int _x2, _y2;		// end square indexes

	bool _castleShort;	// was it kingside castling? 
	bool _castleLong;	// was it queenside castling?
	bool _enPassant;	// was pawn taken en passant?
	int _promoteTo;		// which piece we promoted to?
	bool promote;

public:
	Move(void);
	Move(int x1, int y1, int x2, int y2);
	~Move(void);
 
	// TODO (1)
	//
	// Sets move from string description, e.g.
	//
	// "e2-e4"		- set coordinates
	// "O-O"		- set castle short
	// "O-O-O"		- set castle long
	// "e5-d6ep"	- set coordinates and en passant
	// "e7-e8Q"		- set coordinates and promote to queen
	//
	void strToMove(const char* str);

	// TODO (2)
	//
	// Prints move in ascii, e.g. "e2-e4". For testing.
	//
	void print() const;	

	// Setters and getters
	int getX1() const;
	int getY1() const;
	int getX2() const;
	int getY2() const;

	//Promote
	int getPromoteTo() const;
	bool promoting() const;

	//castling
	bool Castling() const;
	bool castlingShort() const;
	bool castlingLong() const;
};

