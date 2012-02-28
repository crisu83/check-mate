#pragma once
#define SQUARES 64

class Board
{
private:
	Square *_squares[8][8];
	__int64 SquareBits[64];
	Position *_position;

public:
	Board(void);
	~Board(void);

	void clear();
	void initPos();
	void render();
	void execMove(const Move *m);
	void fillSquareBits();
	_int64 posToSquare(int x, int y);
	//TODO: Return something plx |
	//							 V
	void SquareBitToPos(__int64 square);

	void setPieceAt(int x, int y, Piece *piece);
	Square *getSquareAt(int x, int y);
	std::string getTurn(void) const;

	Piece *getPieceAt(int x, int y);
	void setPosition(Position *position);
};
