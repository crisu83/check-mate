#pragma once

class Square
{
private:
	int _x;
	int _y;
	int _color;
	bool _empty;
	Piece *_piece;

public:
	Square(void);
	~Square(void);

	void render();

	// Getters and setters
	int getColor();
	bool isEmpty();
	void setColor(int color);
	Piece *getPiece();
	void setPiece(Piece *piece);
};

