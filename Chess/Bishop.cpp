#include "Bishop.h"

Bishop::Bishop(COLORS color) : Piece(color, BISHOP) {
	setColor(color);
	setType(BISHOP);
	sliding = true;
	const int bishopMoves[4][2] = {
		{  1,  1 }, {  1, -1 },
		{ -1,  1 }, { -1, -1 }
	};

	for (size_t i = 0; i < 4; i++) {
		addMove(bishopMoves[i]);
	}
}
