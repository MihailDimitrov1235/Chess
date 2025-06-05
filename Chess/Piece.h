#pragma once
#include "consts.h"

class Piece
{
private:
	const wchar_t* code;
	size_t movesSize;
	size_t movesCapacity;
	COLORS color;
	PIECES type;
	int** moves;
	void copyFrom(const Piece& other);
	void free();
	void resize(size_t newCapacity);
	void allocateMoves();
	void updateCode();
protected:
	bool sliding;
	void addMove(const int move[COORDINATES]);
public:
	Piece();
	Piece(COLORS color, PIECES type);
	Piece(const Piece& other);
	Piece& operator=(const Piece& other);
	virtual Piece* clone() const;
	virtual ~Piece();

	COLORS getColor() const;
	PIECES getType() const;
	const wchar_t* getCode() const;
	bool getSliding() const;
	int** getMoves() const;
	size_t getMovesSize() const;

	void setType(PIECES type);
	void setColor(COLORS color);

	bool isEmpty() const;

	virtual bool canAttack(int fromRow, int fromCol, int toRow, int toCol, Piece* const board[BOARD_SIZE][BOARD_SIZE]) const;
	virtual void validateMove(int fromRow, int fromCol, int toRow, int toCol,
		Piece* const board[BOARD_SIZE][BOARD_SIZE]) const;

};