#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include "Game.h"
#include "consts.h"
#include "utils.h"

using namespace std;

void Game::setupBackRank(COLORS color, int row) {
	state.board[row][0] = Piece(color, ROOK);
	state.board[row][1] = Piece(color, KNIGHT);
	state.board[row][2] = Piece(color, BISHOP);
	state.board[row][3] = Piece(color, QUEEN);
	state.board[row][4] = Piece(color, KING);
	state.board[row][5] = Piece(color, BISHOP);
	state.board[row][6] = Piece(color, KNIGHT);
	state.board[row][7] = Piece(color, ROOK);
}

void Game::setupPawns(COLORS color, int row)
{
	for (int col = 0; col < BOARD_SIZE; col++) {
		state.board[row][col] = Piece(color, PAWN);
	}
}

Game::Game() : validator(state) {
	state.whiteMove = true;

	state.whiteCanCastleLong = true;
	state.whiteCanCastleShort = true;
	state.blackCanCastleLong = true;
	state.blackCanCastleShort = true;

	state.whiteKingRow = 7;
	state.whiteKingCol = 4;
	state.blackKingRow = 0;
	state.blackKingCol = 4;

	for (int row = 0; row < BOARD_SIZE; ++row) {
		for (int col = 0; col < BOARD_SIZE; ++col) {
			state.board[row][col] = Piece();
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
	bool reverse = !state.whiteMove;

	printCols(reverse);

	int pieceColor = BLACK_COLOR;
	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		wcout << (reverse ? i + 1 : BOARD_SIZE - i) << L" ";
		for (size_t j = 0; j < BOARD_SIZE; j++) {
			bool isWhiteSquare = (i + j) % 2 == 0;
			int bgColor = isWhiteSquare ? LIGHT_GRAY_COLOR : DARK_GRAY_COLOR;

			setColor(pieceColor, bgColor);

			Piece piece = reverse ? state.board[BOARD_SIZE - i - 1][BOARD_SIZE - j - 1] : state.board[i][j];
			wcout << piece.getCode();

			if (j != BOARD_SIZE - 1 || piece.getType() == EMPTY_SQUARE)
			{
				wcout << L" ";
			}
		}
		setColor(WHITE_COLOR, BLACK_COLOR);
		if (state.board[i][BOARD_SIZE - 1].getType() != EMPTY_SQUARE) {
			wcout << L" ";
		}
		wcout << L" " << (reverse ? i + 1 : BOARD_SIZE - i) << endl;
	}

	printCols(reverse);
}

bool Game::doesPieceHaveLegalMoves(int row, int col) {
	Piece piece = state.board[row][col];

	for (int r = 0; r < BOARD_SIZE; r++) {
		for (int c = 0; c < BOARD_SIZE; c++) {
			if (r == row && c == col) {
				continue;
			}
			try {
				validator.validateMove(row, col, r, c);
				return true;
			}
			catch (const invalid_argument&) {
				continue;
			}
		}
	}
	return false;
}


bool Game::doesPlayerHaveLegalMoves()
{
	COLORS color = state.whiteMove ? WHITE : BLACK;
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			Piece currentPiece = state.board[row][col];
			if (currentPiece.getColor() != color)
			{
				continue;
			}
			if (doesPieceHaveLegalMoves(row, col))
			{
				return true;
			}
		}
	}
	return false;
}

bool Game::isGameOver()
{
	if (!doesPlayerHaveLegalMoves())
	{
		int kingRow = state.whiteMove ? state.whiteKingRow : state.blackKingRow;
		int kingCol = state.whiteMove ? state.whiteKingCol : state.blackKingCol;
		if (validator.isKingCapturable(kingRow, kingCol)) {
			wcout << (state.whiteMove ? "Black" : "White") << " wins!";
		}
		else {
			wcout << "Game ends in a draw because " << (state.whiteMove ? "White" : "Black") << " has no legal moves.";
		}
		return true;
	}
	/*if (isPosRepeated3Times())
	{
		wcout << "Game ends in a draw due to threefold repetition";
		return true;
	}*/
	return false;
}

void Game::handleCastlingRelatedMove(int rowFrom, int colFrom, int rowTo, int colTo) {
	Piece movingPiece = state.board[rowFrom][colFrom];
	PIECES type = movingPiece.getType();
	COLORS color = movingPiece.getColor();
	if (type == KING) {
		if (color == WHITE) {
			state.whiteCanCastleShort = false;
			state.whiteCanCastleLong = false;

			state.whiteKingRow = rowTo;
			state.whiteKingCol = colTo;

			if (rowFrom == 7 && colFrom == 4 && rowTo == 7 && colTo == 6) {
				state.board[7][5] = state.board[7][7];
				state.board[7][7] = Piece();
			}
			if (rowFrom == 7 && colFrom == 4 && rowTo == 7 && colTo == 2) {
				state.board[7][3] = state.board[7][0];
				state.board[7][0] = Piece();
			}
		}
		else {
			state.blackCanCastleShort = false;
			state.blackCanCastleLong = false;

			state.blackKingRow = rowTo;
			state.blackKingCol = colTo;

			if (rowFrom == 0 && colFrom == 4 && rowTo == 0 && colTo == 6) {
				state.board[0][5] = state.board[0][7];
				state.board[0][7] = Piece();
			}
			if (rowFrom == 0 && colFrom == 4 && rowTo == 0 && colTo == 2) {
				state.board[0][3] = state.board[0][0];
				state.board[0][0] = Piece();
			}
		}
	}

	if (type == ROOK) {
		if (color == WHITE) {
			if (rowFrom == 7 && colFrom == 0) state.whiteCanCastleLong = false;
			if (rowFrom == 7 && colFrom == 7) state.whiteCanCastleShort = false;
		}
		else {
			if (rowFrom == 0 && colFrom == 0) state.blackCanCastleLong = false;
			if (rowFrom == 0 && colFrom == 7) state.blackCanCastleShort = false;
		}
	}

	if (state.board[rowTo][colTo].getType() == ROOK) {
		if (state.board[rowTo][colTo].getColor() == WHITE) {
			if (rowTo == 7 && colTo == 0) state.whiteCanCastleLong = false;
			if (rowTo == 7 && colTo == 7) state.whiteCanCastleShort = false;
		}
		else {
			if (rowTo == 0 && colTo == 0) state.blackCanCastleLong = false;
			if (rowTo == 0 && colTo == 7) state.blackCanCastleShort = false;
		}
	}
}

void Game::handleEnPassantRelatedMove(int rowFrom, int colFrom, int rowTo, int colTo) {
	if (state.board[rowFrom][colFrom].getType() == PAWN && rowTo == state.enPassantSquare[0] && colTo == state.enPassantSquare[1]) {
		state.board[rowFrom][colTo] = Piece();
	}

	if (state.board[rowFrom][colFrom].getType() == PAWN && absVal(rowTo - rowFrom) == 2) {
		state.enPassantSquare[0] = (rowFrom + rowTo) / 2;
		state.enPassantSquare[1] = colFrom;
	}
	else {
		state.enPassantSquare[0] = -1;
		state.enPassantSquare[1] = -1;
	}
}

void Game::makeMove()
{
	const int BUFFER_SIZE = 128;
	char moveFrom[BUFFER_SIZE];
	char moveTo[BUFFER_SIZE];
	wcout << (state.whiteMove ? "White" : "Black") << "'s move (e.g. e2 e4): ";
	cin >> moveFrom >> moveTo;

	validateLocation(moveFrom);
	validateLocation(moveTo);

	int colFrom = getCol(moveFrom);
	int rowFrom = getRow(moveFrom);
	int colTo = getCol(moveTo);
	int rowTo = getRow(moveTo);

	validator.validateMove(rowFrom, colFrom, rowTo, colTo);

	handleCastlingRelatedMove(rowFrom, colFrom, rowTo, colTo);
	handleEnPassantRelatedMove(rowFrom, colFrom, rowTo, colTo);
	state.board[rowTo][colTo] = state.board[rowFrom][colFrom];
	state.board[rowFrom][colFrom] = Piece();

	state.whiteMove = !state.whiteMove;
}

