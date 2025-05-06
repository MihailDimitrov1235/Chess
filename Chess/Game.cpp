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

Game::Game() : whiteMove(true)
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
	Piece king = board[row][col];
	COLORS color = king.getColor();

	// check for enemy king
	const int kingOffsets[8][2] = { {1, 1}, {0, 1}, {-1, 1}, {1, 0}, {-1, 0}, {1, -1}, {0, -1}, {-1, -1} };
	for (size_t i = 0; i < 8; i++)
	{
		int r = row + kingOffsets[i][0];
		int c = col + kingOffsets[i][1];
		if (isSquareEnemyPiece(r, c, KING)) {
			return true;
		}
	}

	// check for enemy pawns
	const int pawnDirection = whiteMove ? -1 : 1;
	if (isSquareEnemyPiece(row + pawnDirection, col + 1, PAWN) || isSquareEnemyPiece(row + pawnDirection, col - 1, PAWN))
	{
		return true;
	}

	//check for enemy knight
	const int knightOffsets[8][2] = { {2, 1}, {1, 2}, {-1, 2}, {-2, 1}, {2, -1}, {1, -2}, {-1, -2}, {-2, -1} };
	for (size_t i = 0; i < 8; i++)
	{
		int r = row + knightOffsets[i][0];
		int c = col + knightOffsets[i][1];
		if (isSquareEnemyPiece(r, c, KNIGHT)) {
			return true;
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
					return true;
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
					return true;
				}
				break;
			}
			r += bishopDirections[i][0];
			c += bishopDirections[i][1];
		}
	}

	return false;
}

bool Game::isGameOver()
{
	return false;
}

void Game::validateKingMove(int rowFrom, int colFrom, int rowTo, int colTo)
{
	Piece toPiece = board[rowTo][colTo];
	board[rowTo][colTo] = board[rowFrom][colFrom];

	if (isKingCapturable(rowTo, colTo))
	{
		board[rowFrom][colFrom] = board[rowTo][colTo];
		board[rowTo][colTo] = toPiece;
		throw invalid_argument("Invalid move. King cannot move into check.");
	}

	board[rowFrom][colFrom] = board[rowTo][colTo];
	board[rowTo][colTo] = toPiece;
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

	board[rowTo][colTo] = board[rowFrom][colFrom];
	board[rowFrom][colFrom] = Piece();

	whiteMove = !whiteMove;
}
