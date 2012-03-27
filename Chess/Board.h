#pragma once

//some constants
#define CASTL  100
#define ENPASS 200
#define CAPT   300
#define PROMO  400
#define SQUARES 64
#define BITBOARDS 18
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
	int historyTable[10];
	int historyIndex;
	ChessTimer chessTimer;
	std::vector<UI64[2]> moves;
	int fiftyMove;
	int captures, enpassants, castlings,promotions,cheks,checkmates;

	UI64 legalMoves[256];

	//Private functions
	void initBitboards();
	std::string Board::movesAsString(std::vector<UI64> move);
	UI64 *makeBoardBackUp();
	double alphaBetaMax( double alpha, double beta, int depth );
	double alphaBetaMin( double alpha, double beta, int depth );
	int bitScanForward(UI64 bb);

public:
	Board(void);
	~Board(void);


	//General helper functions
	void clear();
	void initPos();
	void render();
	void execMove(const Move *m);
	void fillSquareBits();
	void SquareBitToPos(UI64 square);
	void updateBitBoards(Move move, int type);
	void BitBoardToMoves();
	void fiftyMoveRule();
	bool moveIsLegal(Move* _curMove);
	UI64 posToSquare(int x, int y);
	std::string movesAsString(int x1, int y1, int x2, int y2);


	//Getters
	std::vector<std::vector<UI64>> getLegalMoves();
	std::vector<std::string> getMoveStrings();
	Piece *getPieceAt(int x, int y);
	Square *getSquareAt(int x, int y);
	std::string getTurn(void) const;
	int  getPieceTypeAt(int x, int y);
	UI64 getBitBoard( int type );


	//Setters
	void setPosition(Position *position);
    void setPieceAt(int x, int y, Piece *piece);


	//Functions for AI
	void unMake(std::vector<UI64> move);
	Move *wRootSearch();
	Move *bRootSearch();
	void takeBack(UI64 * b);
	void makeMove(std::vector<UI64> move);
	Move *Board::BitBoardToMoves(std::vector<UI64> move);


	//Functions for debugging
	UI64 Perft(int depth);
	void PerftResults();
	void resetPerftCounters();
	UI64 divide(int depth);
	void divided(int depth);
	void superHiddenRenderEmptySquares(int);

};

