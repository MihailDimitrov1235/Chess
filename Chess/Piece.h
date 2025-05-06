#pragma once
#include "consts.h"

class Piece
{
private:
	COLORS color;
	PIECES type;
	const wchar_t* code;

public:
	Piece();
	Piece(COLORS color, PIECES type);

	COLORS getColor() const;
	PIECES getType() const;
	const wchar_t* getCode() const;

	void setType(PIECES type);
	void setColor(COLORS color);

	bool isEmpty() const;
};
