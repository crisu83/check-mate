#pragma once

// Piece types
enum {
	EMPTY = 0,
	W_KING, W_QUEEN, W_ROOK, W_BISHOP, W_KNIGHT, W_PAWN,
	B_KING, B_QUEEN, B_ROOK, B_BISHOP, B_KNIGHT, B_PAWN
};

class Piece
{
private:
	int _type;

public:
	Piece(void);
	Piece(int type);
	~Piece();

	void render();

	// Setters and getters
	int getType();
};


