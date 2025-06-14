#include "MoveValidator.h"
#include "GameState.h"
#include "utils.h"
#include <iostream>
#include "Piece.h"

using namespace std;

MoveValidator::MoveValidator(Piece* (&board)[BOARD_SIZE][BOARD_SIZE], GameState& state) : board(board), state(state) {}

bool MoveValidator::isKingCapturableAt(int row, int col) {
	int kingRow = state.whiteMove ? state.whiteKingRow : state.blackKingRow;
	int kingCol = state.whiteMove ? state.whiteKingCol : state.blackKingCol;

	Piece* king = board[kingRow][kingCol];
	Piece* captured = board[row][col];

	if (kingRow != row || kingCol != col) {
		board[row][col] = king;
		board[kingRow][kingCol] = new Piece();
	}

	COLORS enemyColor = state.whiteMove ? BLACK : WHITE;
	bool result = false;

	for (int i = 0; i < BOARD_SIZE && !result; i++) {
		for (int j = 0; j < BOARD_SIZE; j++) {
			if (board[i][j]->getColor() == enemyColor &&
				board[i][j]->canAttack(i, j, row, col, board)) {
				result = true;
				break;
			}
		}
	}

	if (kingRow != row || kingCol != col) {
		delete board[kingRow][kingCol];
		board[kingRow][kingCol] = king;
		board[row][col] = captured;
	}

	return result;
}

void MoveValidator::validateCastle(int rowFrom, int colFrom, int rowTo, int colTo) {
	Piece* king = board[rowFrom][colFrom];
	COLORS color = king->getColor();

	if (colFrom != 4) {
		throw invalid_argument("Invalid castle: King not on starting square.");
	}

	bool isWhite = (color == WHITE);
	int row = isWhite ? 7 : 0;

	// long castle
	if (colTo == 2)
	{
		if ((isWhite && !state.whiteCanCastleLong) || (!isWhite && !state.blackCanCastleLong)) {
			throw invalid_argument("Cannot castle long.");
		}

		if (!board[row][1]->isEmpty() || !board[row][2]->isEmpty() || !board[row][3]->isEmpty()) {
			throw invalid_argument("Cannot castle through pieces.");
		}

		if (isKingCapturableAt(row, 4) || isKingCapturableAt(row, 3) || isKingCapturableAt(row, 2)) {
			throw invalid_argument("Cannot castle through or into check.");
		}
	}

	// short castle
	else if (colTo == 6)
	{
		if ((isWhite && !state.whiteCanCastleShort) || (!isWhite && !state.blackCanCastleShort)) {
			throw invalid_argument("Cannot castle short.");
		}

		if (!board[row][5]->isEmpty() || !board[row][6]->isEmpty()) {
			throw invalid_argument("Cannot castle through pieces.");
		}
		if (isKingCapturableAt(row, 4) || isKingCapturableAt(row, 5) || isKingCapturableAt(row, 6)) {
			throw invalid_argument("Cannot castle through or into check.");
		}
	}
	else
	{
		throw invalid_argument("Invalid castle target square.");
	}
}

void MoveValidator::validateKingSafety(int rowFrom, int colFrom, int rowTo, int colTo) {
	Piece* movingPiece = board[rowFrom][colFrom];
	Piece* capturedPiece = board[rowTo][colTo];

	if (movingPiece->getType() == KING) {
		if (isKingCapturableAt(rowTo, colTo))
		{
			throw invalid_argument("Invalid move. Your king will be in danger.");
		}
		return;
	}

	board[rowTo][colTo] = movingPiece;
	board[rowFrom][colFrom] = new Piece();

	int kingRow = state.whiteMove ? state.whiteKingRow : state.blackKingRow;
	int kingCol = state.whiteMove ? state.whiteKingCol : state.blackKingCol;

	if (isKingCapturableAt(kingRow, kingCol)) {
		delete board[rowFrom][colFrom];
		board[rowFrom][colFrom] = movingPiece;
		board[rowTo][colTo] = capturedPiece;

		throw invalid_argument("Invalid move. Your king will be in danger.");
	}

	delete board[rowFrom][colFrom];
	board[rowFrom][colFrom] = movingPiece;
	board[rowTo][colTo] = capturedPiece;
}

void MoveValidator::validateMove(int rowFrom, int colFrom, int rowTo, int colTo) {
	if (rowFrom == rowTo && colFrom == colTo) {
		throw invalid_argument("Invalid move. Select 2 different squares.");
	}

	Piece* fromPiece = board[rowFrom][colFrom];
	Piece* toPiece = board[rowTo][colTo];

	if (!((fromPiece->getColor() == WHITE && state.whiteMove) || (fromPiece->getColor() == BLACK && !state.whiteMove))) {
		throw invalid_argument("Invalid move. Move only your own pieces.");
	}

	if (!toPiece->isEmpty() && toPiece->getColor() == fromPiece->getColor()) {
		throw invalid_argument("Invalid move. Cannot capture your own piece.");
	}

	if (fromPiece->getType() == KING && absVal(colFrom - colTo) == 2 && rowFrom == rowTo) {
		validateCastle(rowFrom, colFrom, rowTo, colTo);
	}
	else if (fromPiece->getType() == PAWN && state.enPassantSquare[0] == rowTo && state.enPassantSquare[1] == colTo) {
		int direction = fromPiece->getColor() == WHITE ? -1 : 1;
		int rowDiff = rowTo - rowFrom;
		int colDiff = colTo - colFrom;

		if (absVal(colDiff) != 1 || rowDiff != direction) {
			throw invalid_argument("Invalid pawn capture.");
		}
	}
	else {
		fromPiece->validateMove(rowFrom, colFrom, rowTo, colTo, board);
	}

	validateKingSafety(rowFrom, colFrom, rowTo, colTo);
}
