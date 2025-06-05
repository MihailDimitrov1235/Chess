#include "Piece.h"
#include "consts.h"

void Piece::allocateMoves() {
	if (movesCapacity > 0) {
		moves = new int* [movesCapacity];
		for (size_t i = 0; i < movesCapacity; i++) {
			moves[i] = new int[COORDINATES];
		}
	}
	else {
		moves = nullptr;
	}
}

void Piece::copyFrom(const Piece& other) {
	color = other.color;
	type = other.type;
	code = other.code;
	movesSize = other.movesSize;
	movesCapacity = other.movesCapacity;
	sliding = other.sliding;

	allocateMoves();
	for (size_t i = 0; i < movesSize; i++) {
		for (size_t j = 0; j < COORDINATES; j++) {
			moves[i][j] = other.moves[i][j];
		}
	}
}

void Piece::free() {
	for (size_t i = 0; i < movesSize; i++) {
		delete[] moves[i];
	}
	delete[] moves;
	moves = nullptr;
}

void Piece::resize(size_t newCapacity) {
	int** newMoves = new int* [newCapacity];
	for (size_t i = 0; i < newCapacity; i++) {
		newMoves[i] = new int[COORDINATES];
	}
	for (size_t i = 0; i < movesSize; i++) {
		for (size_t j = 0; j < COORDINATES; j++) {
			newMoves[i][j] = moves[i][j];
		}
	}
	for (size_t i = 0; i < movesSize; i++) {
		delete[] moves[i];
	}
	delete[] moves;
	moves = newMoves;
	movesCapacity = newCapacity;
}


void Piece::addMove(int move[COORDINATES]) {
	if (movesSize >= movesCapacity) {
		resize(movesCapacity * 2);
	}
	for (size_t i = 0; i < COORDINATES; i++) {
		moves[movesSize][i] = move[i];
	}
	movesSize++;
}

Piece::Piece() : color(NONE), type(EMPTY_SQUARE), code(L" "), movesSize(0), movesCapacity(4), sliding(false), moves(nullptr) {
	allocateMoves();
}

Piece::Piece(COLORS color, PIECES type) : color(color), type(type), movesSize(0), movesCapacity(4), sliding(false) {
	allocateMoves();
	if (color == WHITE)
		code = WHITE_PIECES[type];
	else if (color == BLACK)
		code = BLACK_PIECES[type];
	else
		code = L" ";
}

Piece::Piece(const Piece& other) {
	copyFrom(other);
}

Piece& Piece::operator=(const Piece& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

Piece::~Piece() {
	free();
}

COLORS Piece::getColor() const { return color; }
PIECES Piece::getType() const { return type; }
const wchar_t* Piece::getCode() const { return code; }

int** Piece::getMoves() const {
	return moves;
}

size_t Piece::getMovesSize() const {
	return movesSize;
}

void Piece::setType(PIECES type) {
	this->type = type;
	if (color == WHITE)
		code = WHITE_PIECES[type];
	else if (color == BLACK)
		code = BLACK_PIECES[type];
}

void Piece::setColor(COLORS color) {
	this->color = color;
	if (type != EMPTY_SQUARE) {
		if (color == WHITE)
			code = WHITE_PIECES[type];
		else if (color == BLACK)
			code = BLACK_PIECES[type];
	}
}

bool Piece::isEmpty() const {
	return color == NONE || type == EMPTY_SQUARE || movesSize == 0;
}
