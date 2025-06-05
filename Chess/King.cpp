#include "King.h"

King::King(COLORS color) : Piece(color, KING) {
	sliding = false;
	const int kingMoves[10][2] = {
		{  1,  1 }, {  1, 0 },
		{ 1,  -1 }, { 0, -1 },
		{ -1,  -1 }, { -1, 0 },
		{ -1,  1 }, { 0, 1 },
		{ 0,  2 }, { 0, -2 }
	};

	for (size_t i = 0; i < 10; i++) {
		addMove(kingMoves[i]);
	}
}
