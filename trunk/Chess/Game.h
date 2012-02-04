#pragma once

#define WHITE 0
#define BLACK 1

class Game
{
private:
	bool _running;
	int _turnNum;
	int _toMove;
	Board *_board;
	Position *_position;
	Move *_curMove;

public:
	Game(void);
	~Game(void);

	void init();
	int run();
	void update();
	void render();
	void endTurn();
	std::string getTurnName() const;
};

