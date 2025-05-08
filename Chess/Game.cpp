#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include "Game.h"
#include "consts.h"
#include "utils.h"

using namespace std;

void Game::setupBackRank(COLORS color, int row) {
	board[row][0] = Piece(color, ROOK);
	board[row][1] = Piece(color, KNIGHT);
	board[row][2] = Piece(color, BISHOP);
	board[row][3] = Piece(color, QUEEN);
	board[row][4] = Piece(color, KING);
	board[row][5] = Piece(color, BISHOP);
	board[row][6] = Piece(color, KNIGHT);
	board[row][7] = Piece(color, ROOK);
}

void Game::setupPawns(COLORS color, int row)
{
	for (int col = 0; col < BOARD_SIZE; col++) {
		board[row][col] = Piece(color, PAWN);
	}
}

Game::Game() : whiteMove(true),
whiteCanCastleLong(true),
whiteCanCastleShort(true),
blackCanCastleLong(true),
blackCanCastleShort(true),
whiteInCheck(false),
blackInCheck(false),
whiteKingRow(7),
whiteKingCol(4),
blackKingRow(0),
blackKingCol(4)
{
	for (int row = 0; row < BOARD_SIZE; ++row) {
		for (int col = 0; col < BOARD_SIZE; ++col) {
			board[row][col] = Piece();
		}
	}

	setupBackRank(BLACK, 0);
	setupPawns(BLACK, 1);
	setupPawns(WHITE, BOARD_SIZE - 2);
	setupBackRank(WHITE, BOARD_SIZE - 1);
}

void Game::printCols(bool reverse)
{
	wcout << L"  ";
	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		char letter = reverse ? 'a' + BOARD_SIZE - i - 1 : 'a' + i;
		wcout << letter << L" ";
	}
	wcout << endl;
}

void Game::printBoard()
{
	bool reverse = !whiteMove;

	printCols(reverse);

	int pieceColor = BLACK_COLOR;
	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		wcout << (reverse ? i + 1 : BOARD_SIZE - i) << L" ";
		for (size_t j = 0; j < BOARD_SIZE; j++) {
			bool isWhiteSquare = (i + j) % 2 == 0;
			int bgColor = isWhiteSquare ? LIGHT_GRAY_COLOR : DARK_GRAY_COLOR;

			setColor(pieceColor, bgColor);

			Piece piece = reverse ? board[BOARD_SIZE - i - 1][BOARD_SIZE - j - 1] : board[i][j];
			wcout << piece.getCode();

			if (j != BOARD_SIZE - 1 || piece.getType() == EMPTY_SQUARE)
			{
				wcout << L" ";
			}
		}
		setColor(WHITE_COLOR, BLACK_COLOR);
		if (board[i][BOARD_SIZE - 1].getType() != EMPTY_SQUARE) {
			wcout << L" ";
		}
		wcout << L" " << (reverse ? i + 1 : BOARD_SIZE - i) << endl;
	}

	printCols(reverse);
}

bool Game::isSquareEnemyPiece(int row, int col, PIECES type) {
	if (row < 0 || row >= BOARD_SIZE || col < 0 || col >= BOARD_SIZE)
	{
		return false;
	}
	COLORS enemyColor = whiteMove ? BLACK : WHITE;
	return board[row][col].getType() == type && board[row][col].getColor() == enemyColor;
}

bool Game::isKingCapturable(int row, int col)
{
	int kingRow = whiteMove ? whiteKingRow : blackKingRow;
	int kingCol = whiteMove ? whiteKingCol : blackKingCol;
	Piece king = board[kingRow][kingCol];
	board[kingRow][kingCol] = Piece();

	bool result = false;

	// check for enemy king
	const int kingOffsets[8][2] = { {1, 1}, {0, 1}, {-1, 1}, {1, 0}, {-1, 0}, {1, -1}, {0, -1}, {-1, -1} };
	for (size_t i = 0; i < 8; i++)
	{
		int r = row + kingOffsets[i][0];
		int c = col + kingOffsets[i][1];
		if (isSquareEnemyPiece(r, c, KING)) {
			result = true;
		}
	}

	// check for enemy pawns
	const int pawnDirection = whiteMove ? -1 : 1;
	if (isSquareEnemyPiece(row + pawnDirection, col + 1, PAWN) || isSquareEnemyPiece(row + pawnDirection, col - 1, PAWN))
	{
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

bool Game::isGameOver()
{
	return false;
}

void Game::validateCastle(int rowFrom, int colFrom, int rowTo, int colTo)
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
		if ((isWhite && !whiteCanCastleLong) || (!isWhite && !blackCanCastleLong)) {
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
		if ((isWhite && !whiteCanCastleShort) || (!isWhite && !blackCanCastleShort)) {
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


void Game::validateKingMove(int rowFrom, int colFrom, int rowTo, int colTo)
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

void Game::validateMove(int rowFrom, int colFrom, int rowTo, int colTo)
{
	Piece fromPiece = board[rowFrom][colFrom];
	Piece toPiece = board[rowTo][colTo];

	if (!((fromPiece.getColor() == WHITE && whiteMove) || (fromPiece.getColor() == BLACK && !whiteMove)))
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
		//validatePawnMove();
		break;
	case ROOK:
		//validatePawnMove();
		break;
	case BISHOP:
		//validatePawnMove();
		break;
	case KNIGHT:
		//validatePawnMove();
		break;
	case PAWN:
		//validatePawnMove();
		break;
	default:
		throw logic_error("Unknown piece type.");
	}

}

void Game::handleCastlingRelatedMove(int rowFrom, int colFrom, int rowTo, int colTo) {
	Piece movingPiece = board[rowFrom][colFrom];
	PIECES type = movingPiece.getType();
	COLORS color = movingPiece.getColor();
	if (type == KING) {
		if (color == WHITE) {
			whiteCanCastleShort = false;
			whiteCanCastleLong = false;

			whiteKingRow = rowTo;
			whiteKingCol = colTo;

			if (rowFrom == 7 && colFrom == 4 && rowTo == 7 && colTo == 6) {
				board[7][5] = board[7][7];
				board[7][7] = Piece();
			}
			if (rowFrom == 7 && colFrom == 4 && rowTo == 7 && colTo == 2) {
				board[7][3] = board[7][0];
				board[7][0] = Piece();
			}
		}
		else {
			blackCanCastleShort = false;
			blackCanCastleLong = false;

			blackKingRow = rowTo;
			blackKingCol = colTo;

			if (rowFrom == 0 && colFrom == 4 && rowTo == 0 && colTo == 6) {
				board[0][5] = board[0][7];
				board[0][7] = Piece();
			}
			if (rowFrom == 0 && colFrom == 4 && rowTo == 0 && colTo == 2) {
				board[0][3] = board[0][0];
				board[0][0] = Piece();
			}
		}
	}

	if (type == ROOK) {
		if (color == WHITE) {
			if (rowFrom == 7 && colFrom == 0) whiteCanCastleLong = false;
			if (rowFrom == 7 && colFrom == 7) whiteCanCastleShort = false;
		}
		else {
			if (rowFrom == 0 && colFrom == 0) blackCanCastleLong = false;
			if (rowFrom == 0 && colFrom == 7) blackCanCastleShort = false;
		}
	}

	if (board[rowTo][colTo].getType() == ROOK) {
		if (board[rowTo][colTo].getColor() == WHITE) {
			if (rowTo == 7 && colTo == 0) whiteCanCastleLong = false;
			if (rowTo == 7 && colTo == 7) whiteCanCastleShort = false;
		}
		else {
			if (rowTo == 0 && colTo == 0) blackCanCastleLong = false;
			if (rowTo == 0 && colTo == 7) blackCanCastleShort = false;
		}
	}
}

void Game::makeMove()
{
	const int BUFFER_SIZE = 128;
	char moveFrom[BUFFER_SIZE];
	char moveTo[BUFFER_SIZE];
	wcout << (whiteMove ? "White" : "Black") << "'s move (e.g. e2 e4): ";
	cin >> moveFrom >> moveTo;

	validateLocation(moveFrom);
	validateLocation(moveTo);

	int colFrom = getCol(moveFrom);
	int rowFrom = getRow(moveFrom);
	int colTo = getCol(moveTo);
	int rowTo = getRow(moveTo);

	validateMove(rowFrom, colFrom, rowTo, colTo);

	handleCastlingRelatedMove(rowFrom, colFrom, rowTo, colTo);
	board[rowTo][colTo] = board[rowFrom][colFrom];
	board[rowFrom][colFrom] = Piece();

	whiteMove = !whiteMove;
}

