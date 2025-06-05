#include "Bishop.h"

Bishop::Bishop(COLORS color) : Piece(color, BISHOP) {
	sliding = true;
	int bishopMoves[4][2] = {
		{  1,  1 }, {  1, -1 },
		{ -1,  1 }, { -1, -1 }
	};

	for (size_t i = 0; i < movesSize; i++) {
		addMove(bishopMoves[i]);
	}
}
