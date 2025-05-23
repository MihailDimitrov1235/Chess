#include "MoveValidator.h"
#include "GameState.h"
#include "utils.h"
#include <iostream>
#include "Piece.h"

using namespace std;

MoveValidator::MoveValidator(Piece(&board)[BOARD_SIZE][BOARD_SIZE], GameState& state) : board(board), state(state) {}

bool MoveValidator::isSquareEnemyPiece(int row, int col, PIECES type) {
	if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE) {
		return false;
	}
	COLORS enemyColor = state.whiteMove ? BLACK : WHITE;
	return board[row][col].getType() == type && board[row][col].getColor() == enemyColor;
}

bool MoveValidator::isKingCapturable(int row, int col)
{
	int kingRow = state.whiteMove ? state.whiteKingRow : state.blackKingRow;
	int kingCol = state.whiteMove ? state.whiteKingCol : state.blackKingCol;
	Piece king = board[kingRow][kingCol];
	board[kingRow][kingCol] = Piece();

	bool result = false;

	// check for enemy king
	const int kingOffsets[8][2] = { {1, 1}, {0, 1}, {-1, 1}, {1, 0}, {-1, 0}, {1, -1}, {0, -1}, {-1, -1} };
	for (size_t i = 0; i < 8; i++) {
		int r = row + kingOffsets[i][0];
		int c = col + kingOffsets[i][1];
		if (isSquareEnemyPiece(r, c, KING)) {
			result = true;
		}
	}

	// check for enemy pawns
	const int pawnDirection = state.whiteMove ? -1 : 1;
	if (isSquareEnemyPiece(row + pawnDirection, col + 1, PAWN) || isSquareEnemyPiece(row + pawnDirection, col - 1, PAWN)) {
		result = true;
	}

	//check for enemy knight
	const int knightOffsets[8][2] = { {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1} };
	for (size_t i = 0; i < 8; i++)
	{
		int r = row + knightOffsets[i][0];
		int c = col + knightOffsets[i][1];
		if (isSquareEnemyPiece(r, c, KNIGHT)) {
			result = true;
		}
	}

	//check for rooks/queens
	const int rookDirections[4][2] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };
	for (size_t i = 0; i < 4; i++)
	{
		int r = row + rookDirections[i][0];
		int c = col + rookDirections[i][1];
		while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {

			if (!board[r][c].isEmpty())
			{
				if (isSquareEnemyPiece(r, c, QUEEN) || isSquareEnemyPiece(r, c, ROOK)) {
					result = true;
				}
				break;
			}
			r += rookDirections[i][0];
			c += rookDirections[i][1];
		}
	}

	//check for bishops/queens
	const int bishopDirections[4][2] = { {1, 1}, {-1, 1}, {1, -1}, {-1, -1} };
	for (size_t i = 0; i < 4; i++)
	{
		int r = row + bishopDirections[i][0];
		int c = col + bishopDirections[i][1];
		while (r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE) {

			if (!board[r][c].isEmpty())
			{
				if (isSquareEnemyPiece(r, c, QUEEN) || isSquareEnemyPiece(r, c, BISHOP)) {
					result = true;
				}
				break;
			}
			r += bishopDirections[i][0];
			c += bishopDirections[i][1];
		}
	}
	board[kingRow][kingCol] = king;
	return result;
}

void MoveValidator::validateCastle(int rowFrom, int colFrom, int rowTo, int colTo)
{
	Piece king = board[rowFrom][colFrom];
	COLORS color = king.getColor();

	if (colFrom != 4)
		throw invalid_argument("Invalid castle: King not on starting square.");

	bool isWhite = (color == WHITE);
	int row = isWhite ? 7 : 0;

	// long castle
	if (colTo == 2)
	{
		if ((isWhite && !state.whiteCanCastleLong) || (!isWhite && !state.blackCanCastleLong)) {
			throw invalid_argument("Cannot castle long.");
		}

		if (!board[row][1].isEmpty() || !board[row][2].isEmpty() || !board[row][3].isEmpty()) {
			throw invalid_argument("Cannot castle through pieces.");
		}

		if (isKingCapturable(row, 4) || isKingCapturable(row, 3) || isKingCapturable(row, 2)) {
			throw invalid_argument("Cannot castle through or into check.");
		}
	}

	// short castle
	else if (colTo == 6)
	{
		if ((isWhite && !state.whiteCanCastleShort) || (!isWhite && !state.blackCanCastleShort)) {
			throw invalid_argument("Cannot castle short.");
		}

		if (!board[row][5].isEmpty() || !board[row][6].isEmpty()) {
			throw invalid_argument("Cannot castle through pieces.");
		}
		if (isKingCapturable(row, 4) || isKingCapturable(row, 5) || isKingCapturable(row, 6)) {
			throw invalid_argument("Cannot castle through or into check.");
		}
	}
	else
	{
		throw invalid_argument("Invalid castle target square.");
	}
}

void MoveValidator::validateKingMove(int rowFrom, int colFrom, int rowTo, int colTo)
{
	if (absVal(colFrom - colTo) == 2 && rowFrom == rowTo) {
		validateCastle(rowFrom, colFrom, rowTo, colTo);
		return;
	}
	if (absVal(colFrom - colTo) > 1 || absVal(rowFrom - rowTo) > 1)
	{
		throw invalid_argument("Invalid move. King can only move to adjacent squares.");
	}
	if (isKingCapturable(rowTo, colTo))
	{
		throw invalid_argument("Invalid move. King cannot move into check.");
	}
}

void MoveValidator::validateSlidingMove(int rowFrom, int colFrom, int rowTo, int colTo, bool allowDiagonal, bool allowStraight) {
	int rowDiff = rowTo - rowFrom;
	int colDiff = colTo - colFrom;

	bool isDiagonal = absVal(rowDiff) == absVal(colDiff);
	bool isStraight = rowFrom == rowTo || colFrom == colTo;

	if (!((allowStraight && isStraight) || (allowDiagonal && isDiagonal))) {
		if (allowStraight && !allowDiagonal) {
			throw invalid_argument("Invalid rook move. Move must be a straight line.");
		}
		if (!allowStraight && allowDiagonal) {
			throw invalid_argument("Invalid bishop move. Move must be a diagonal line.");
		}
		throw invalid_argument("Invalid queen move. Move must be a diagonal or straight line.");
	}

	int rowDir = (rowDiff == 0) ? 0 : (rowDiff > 0 ? 1 : -1);
	int colDir = (colDiff == 0) ? 0 : (colDiff > 0 ? 1 : -1);

	int r = rowFrom + rowDir;
	int c = colFrom + colDir;

	while (r != rowTo || c != colTo) {
		if (!board[r][c].isEmpty()) {
			throw invalid_argument("Invalid move. Path is blocked.");
		}
		r += rowDir;
		c += colDir;
	}
}

void MoveValidator::validateKnightMove(int rowFrom, int colFrom, int rowTo, int colTo)
{
	int rowDiff = absVal(rowTo - rowFrom);
	int colDiff = absVal(colTo - colFrom);

	if (!((rowDiff == 1 && colDiff == 2) || (rowDiff == 2 && colDiff == 1)))
	{
		throw invalid_argument("Invalid knight move. Must move in an L-shape.");
	}
}

void MoveValidator::validatePawnMove(int rowFrom, int colFrom, int rowTo, int colTo) {
	int startingRow = state.whiteMove ? 6 : 1;
	int direction = state.whiteMove ? -1 : 1;

	int rowDiff = rowTo - rowFrom;
	int colDiff = colTo - colFrom;

	if (absVal(colDiff) == 1 && rowDiff == direction) {
		if (!board[rowTo][colTo].isEmpty()) {
			return;
		}
		if (state.enPassantSquare[0] == rowTo && state.enPassantSquare[1] == colTo)
		{
			return;
		}
		throw invalid_argument("Invalid pawn move. Diagonal move must capture.");
	}

	if (colDiff == 0) {
		if (!board[rowTo][colTo].isEmpty()) {
			throw invalid_argument("Invalid pawn move. Cannot capture vertically.");
		}
		if (rowDiff == direction) {
			return;
		}
		if (rowDiff == 2 * direction && rowFrom == startingRow) {
			int midRow = rowFrom + direction;
			if (!board[midRow][colFrom].isEmpty()) {
				throw invalid_argument("Invalid pawn move. Cannot jump over piece.");
			}
			return;
		}
		throw invalid_argument("Invalid pawn move. Invalid vertical position.");
	}

	throw invalid_argument("Invalid pawn move. Pawns can only move forward or capture diagonally.");
}

void MoveValidator::validateKingSafety(int rowFrom, int colFrom, int rowTo, int colTo)
{
	Piece toPiece = board[rowTo][colTo];
	board[rowTo][colTo] = board[rowFrom][colFrom];
	board[rowFrom][colFrom] = Piece();

	int kingRow = state.whiteMove ? state.whiteKingRow : state.blackKingRow;
	int kingCol = state.whiteMove ? state.whiteKingCol : state.blackKingCol;
	if (isKingCapturable(kingRow, kingCol))
	{
		board[rowFrom][colFrom] = board[rowTo][colTo];
		board[rowTo][colTo] = toPiece;
		throw invalid_argument("Invalid move. Your king will be in danger.");
	}
	board[rowFrom][colFrom] = board[rowTo][colTo];
	board[rowTo][colTo] = toPiece;
}

void MoveValidator::validateMove(int rowFrom, int colFrom, int rowTo, int colTo)
{
	if (rowFrom == rowTo && colFrom == colTo)
	{
		throw invalid_argument("Invalid move. Select 2 different squares.");
	}

	Piece fromPiece = board[rowFrom][colFrom];
	Piece toPiece = board[rowTo][colTo];

	if (!((fromPiece.getColor() == WHITE && state.whiteMove) || (fromPiece.getColor() == BLACK && !state.whiteMove)))
	{
		throw invalid_argument("Invalid move. Move only your own pieces.");
	}

	if (!toPiece.isEmpty() && toPiece.getColor() == fromPiece.getColor()) {
		throw invalid_argument("Invalid move. Cannot capture your own piece.");
	}

	switch (fromPiece.getType()) {
	case KING:
		validateKingMove(rowFrom, colFrom, rowTo, colTo);
		break;
	case QUEEN:
		validateSlidingMove(rowFrom, colFrom, rowTo, colTo, true, true);
		break;
	case ROOK:
		validateSlidingMove(rowFrom, colFrom, rowTo, colTo, false, true);
		break;
	case BISHOP:
		validateSlidingMove(rowFrom, colFrom, rowTo, colTo, true, false);
		break;
	case KNIGHT:
		validateKnightMove(rowFrom, colFrom, rowTo, colTo);
		break;
	case PAWN:
		validatePawnMove(rowFrom, colFrom, rowTo, colTo);
		break;
	default:
		throw logic_error("Unknown piece type.");
	}

	if (fromPiece.getType() != KING)
	{
		validateKingSafety(rowFrom, colFrom, rowTo, colTo);
	}
}
