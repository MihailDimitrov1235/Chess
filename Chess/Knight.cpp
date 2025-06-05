#include "Knight.h"

Knight::Knight(COLORS color) : Piece(color, KNIGHT) {
	sliding = false;
	const int knightMoves[8][2] = {
		{  2,  1 }, {  2, -1 },
		{ -2,  1 }, { -2, -1 },
		{  1,  2 }, {  1, -2 },
		{ -1,  2 }, { -1, -2 }
	};

	for (size_t i = 0; i < 8; i++) {
		addMove(knightMoves[i]);
	}
}
