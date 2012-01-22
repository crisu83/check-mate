#pragma once

class Board
{
private:
	Square *_squares[8][8];

public:
	Board(void);
	~Board(void);

	void clear();
	void initPos();
	void render();
	void execMove(const Move *m);
	void setPieceAt(int x, int y, Piece *piece);
	Square *getSquareAt(int x, int y);
	std::string getTurn(void) const;

	Piece *getPieceAt(int x, int y);
};

