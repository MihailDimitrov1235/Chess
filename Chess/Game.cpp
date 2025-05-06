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
