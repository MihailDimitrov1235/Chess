#include "Rook.h"

Rook::Rook(COLORS color) {
	setColor(color);
	setType(ROOK);
	sliding = true;
	const int rookMoves[4][2] = {
		{  1,  0 }, {  -1, 0 },
		{ 0,  1 }, { 0, -1 }
	};

	for (size_t i = 0; i < 4; i++) {
		addMove(rookMoves[i]);
	}
}
