#pragma once

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

