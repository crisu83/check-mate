#pragma once

//some constants
#define SQUARES 64
#define BITBOARDS 17
#define A_FILE		0x0101010101010101
#define B_FILE		0x202020202020202
#define C_FILE		0x404040404040404
#define D_FILE		0x808080808080808
#define E_FILE		0x1010101010101010
#define F_FILE		0x2020202020202020
#define G_FILE		0x4040404040404040
#define H_FILE		0x8080808080808080
#define FIRST_RANK	0x00000000000000ff
#define EIGHT_RANK  0xFF00000000000000
#define THIRD_RANK	0x0000000000ff0000
#define SIXTH_RANK  0x0000FF0000000000
#define D1C1B1_MASK 0x000000000000000E
#define F1G1_MASK   0x0000000000000060
#define F8G8_MASK	0x6000000000000000
#define D8C8B8_MASK 0xE00000000000000

class Board
{
private:
	Square *_squares[8][8];
	UI64 _SquareBits[SQUARES];
	Position *_position;
	UI64 _BitBoards[BITBOARDS];
	UI64 _backUp[ BITBOARDS ];

	//Private functions
	void initBitboards();
	std::vector<UI64[2]> moves;
	int fiftyMove;

	ChessTimer chessTimer;

	UI64 *makeBoardBackUp();

	int alphaBetaMax( int alpha, int beta, int depth );
	int alphaBetaMin( int alpha, int beta, int depth );
	int bitScanForward(UI64 bb);

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
	bool moveIsLegal(Move* _curMove);
	void updateBitBoards(Move move, int type);
	int  getPieceTypeAt(int x, int y);
	UI64 getBitBoard( int type );
	void setPieceAt(int x, int y, Piece *piece);
	Square *getSquareAt(int x, int y);
	std::string getTurn(void) const;

	void BitBoardToMoves();
	Move *Board::BitBoardToMoves(std::vector<UI64> move);
	void fiftyMoveRule();


	Piece *getPieceAt(int x, int y);
	void setPosition(Position *position);
	std::vector<std::vector<UI64>> getLegalMoves();
	
	void makeMove(std::vector<UI64> move);
	void takeBack(UI64 * b);
	std::vector<std::string> getMoveStrings();
	std::string movesAsString(int x1, int y1, int x2, int y2);

	Move *wRootSearch();
	Move *bRootSearch();

	UI64 Perft(int depth);
	int divide(int depth);
};

