#pragma once

class Piece
{
private:
	int _x;
	int _y;
	int _type;

public:
	Piece(void);
	Piece(int type, int x, int y);
	~Piece(void);

	int genLegalMoves(Move *moveList);

	int getType() const;
};

