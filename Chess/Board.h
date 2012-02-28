#pragma once
#define SQUARES 64
#define BITBOARDS 15

class Board
{
private:
	Square *_squares[8][8];
	UI64 _SquareBits[SQUARES];
	Position *_position;
	UI64 _BitBoards[BITBOARDS];

	//Private functions
	void initBitboards();

public:
	Board(void);
	~Board(void);

	void clear();
	void initPos();
	void render();
	void execMove(const Move *m);
	void fillSquareBits();
	UI64 posToSquare(int x, int y);
	//TODO: Return something plx |
	//							 V
	void SquareBitToPos(UI64 square);
	bool moveIsLegal(Move _curMove);
	void Board::updateBitBoards(Move move, int type);
	int Board::getPieceTypeAt(int x, int y);
	
	void setPieceAt(int x, int y, Piece *piece);
	Square *getSquareAt(int x, int y);
	std::string getTurn(void) const;

	Piece *getPieceAt(int x, int y);
	void setPosition(Position *position);

};

