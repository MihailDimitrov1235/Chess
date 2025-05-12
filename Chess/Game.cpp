#include <iostream>
#include <fstream>
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

char* Game::encodeBoard()
{
	char* result = new char[BOARD_SIZE * BOARD_SIZE + 1];
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			int index = row * BOARD_SIZE + col;
			Piece piece = state.board[row][col];
			char symbol = piece.getType() + 'A';
			result[index] = piece.getColor() == WHITE ? symbol : toLower(symbol);
		}
	}
	result[BOARD_SIZE * BOARD_SIZE] = '\0';
	return result;
}

void Game::freePositionsMemory() {
	for (size_t i = 0; i < prevPos.positionsSize; i++)
	{
		delete[] prevPos.positions[i];
	}
	delete[] prevPos.positions;
	delete[] prevPos.positionsCounter;
	prevPos.positionsSize = 0;
}

void Game::copyFrom(const Game& other) {
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			state.board[row][col] = other.state.board[row][col];
		}
	}

	state.whiteMove = other.state.whiteMove;
	state.whiteCanCastleLong = other.state.whiteCanCastleLong;
	state.whiteCanCastleShort = other.state.whiteCanCastleShort;
	state.blackCanCastleLong = other.state.blackCanCastleLong;
	state.blackCanCastleShort = other.state.blackCanCastleShort;

	state.whiteKingRow = other.state.whiteKingRow;
	state.whiteKingCol = other.state.whiteKingCol;
	state.blackKingRow = other.state.blackKingRow;
	state.blackKingCol = other.state.blackKingCol;

	state.enPassantSquare[0] = other.state.enPassantSquare[0];
	state.enPassantSquare[1] = other.state.enPassantSquare[1];

	prevPos.positionsSize = other.prevPos.positionsSize;
	if (prevPos.positionsSize > 0) {
		prevPos.positions = new char* [prevPos.positionsSize];
		prevPos.positionsCounter = new int[prevPos.positionsSize];

		for (int i = 0; i < prevPos.positionsSize; ++i) {
			prevPos.positions[i] = new char[BOARD_SIZE * BOARD_SIZE + 1];
			strcpy_s(prevPos.positions[i], BOARD_SIZE * BOARD_SIZE + 1, other.prevPos.positions[i]);
			prevPos.positionsCounter[i] = other.prevPos.positionsCounter[i];
		}
	}
	else {
		prevPos.positions = nullptr;
		prevPos.positionsCounter = nullptr;
	}
}


void Game::savePosition()
{
	char* newPosition = encodeBoard();
	for (size_t i = 0; i < prevPos.positionsSize; i++)
	{
		if (compareStrs(prevPos.positions[i], newPosition)) {
			prevPos.positionsCounter[i] += 1;
			delete[] newPosition;
			return;
		}
	}
	int newSize = prevPos.positionsSize + 1;
	char** newPositions = new char* [newSize];
	int* newPositionsCounter = new int[newSize];
	for (size_t i = 0; i < prevPos.positionsSize; i++)
	{
		newPositions[i] = new char[BOARD_SIZE * BOARD_SIZE + 1];
		strcpy_s(newPositions[i], BOARD_SIZE * BOARD_SIZE + 1, prevPos.positions[i]);
		newPositionsCounter[i] = prevPos.positionsCounter[i];
	}
	newPositions[newSize - 1] = newPosition;
	newPositionsCounter[newSize - 1] = 1;
	freePositionsMemory();
	prevPos.positions = newPositions;
	prevPos.positionsCounter = newPositionsCounter;
	prevPos.positionsSize = newSize;
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

	savePosition();
}

Game::Game(const Game& other) : validator(state) {
	copyFrom(other);
}

Game& Game::operator=(const Game& other)
{
	if (this != &other)
	{
		freePositionsMemory();
		copyFrom(other);
	}
	return *this;
}

Game::~Game()
{
	freePositionsMemory();
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
	wcout << "\033[2J\033[H"; // clear console and start top left

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

bool Game::hasPiecesForMate()
{
	int blackMinorPieces = 0;
	COLORS blackBishopSquareColor = NONE;
	int whiteMinorPieces = 0;
	COLORS whiteBishopSquareColor = NONE;
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			Piece piece = state.board[row][col];
			if (piece.isEmpty() || piece.getType() == KING) {
				continue;
			}
			PIECES type = piece.getType();
			if (type == PAWN || type == ROOK || type == QUEEN) {
				return true;
			}
			COLORS squareColor = (row + col) % 2 == 0 ? WHITE : BLACK;
			if (piece.getColor() == WHITE) {
				if (type == KNIGHT || (type == BISHOP && squareColor != whiteBishopSquareColor))
				{
					whiteMinorPieces++;
				}
				if (type == BISHOP)
				{
					whiteBishopSquareColor = squareColor;
				}
			}
			else {
				if (type == KNIGHT || (type == BISHOP && squareColor != blackBishopSquareColor))
				{
					blackMinorPieces++;
				}
				if (type == BISHOP)
				{
					blackBishopSquareColor = squareColor;
				}
			}

		}
	}
	if (whiteMinorPieces >= 2 || blackMinorPieces >= 2)
	{
		return true;
	}
	return false;
}

bool Game::hasThreefoldRepetition()
{
	for (size_t i = 0; i < prevPos.positionsSize; i++)
	{
		if (prevPos.positionsCounter[i] >= 3) {
			return true;
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
	if (!hasPiecesForMate()) {
		wcout << "Game ends in a draw because of lack of pieces for checkmate.";
	}
	if (hasThreefoldRepetition())
	{
		wcout << "Game ends in a draw due to threefold repetition.";
		return true;
	}
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

void Game::handlePromotion(int row, int col)
{
	int promotionRow = state.whiteMove ? 0 : 7;
	COLORS color = state.whiteMove ? WHITE : BLACK;
	if (state.board[row][col].getType() == PAWN && promotionRow == row)
	{
		wcout << "Promote pawn into: \n(1) Queen \n(2) Rook \n(3) Bishop \n(4) Knight\n";
		int selectedPromotion;
		bool promotionSelected = false;
		while (!promotionSelected) {
			try {
				cin >> selectedPromotion;
				if (selectedPromotion < 1 || selectedPromotion > 4)
				{
					throw invalid_argument("Select number between 1 and 4:");
				}
				promotionSelected = true;
			}
			catch (const exception& e) {
				wcout << e.what() << endl;
				fixCin();
			}
		}
		switch (selectedPromotion)
		{
		case 1:
			state.board[row][col] = Piece(color, QUEEN);
			break;
		case 2:
			state.board[row][col] = Piece(color, ROOK);
			break;
		case 3:
			state.board[row][col] = Piece(color, BISHOP);
			break;
		case 4:
			state.board[row][col] = Piece(color, KNIGHT);
			break;
		default:
			throw logic_error("Invalid promotion piece selected.");
			break;
		}
	}
}

void Game::saveGame()
{
	ofstream outFile(fileName, ios::binary);
	if (!outFile) {
		throw runtime_error("Could not open save file.");
	}
	outFile.write(reinterpret_cast<const char*>(&state), sizeof(GameState));
	outFile.write(reinterpret_cast<const char*>(&prevPos.positionsSize), sizeof(int));
	for (int i = 0; i < prevPos.positionsSize; i++) {
		outFile.write(prevPos.positions[i], BOARD_SIZE * BOARD_SIZE);
		outFile.write(reinterpret_cast<const char*>(&prevPos.positionsCounter[i]), sizeof(int));
	}
	outFile.close();
}

void Game::loadGame()
{
	ifstream inFile(fileName, ios::binary);
	if (!inFile) {
		throw runtime_error("Could not open save file.");
	}
	inFile.read(reinterpret_cast<char*>(&state), sizeof(GameState));
	inFile.read(reinterpret_cast<char*>(&prevPos.positionsSize), sizeof(int));
	prevPos.positions = new char* [prevPos.positionsSize];
	for (int i = 0; i < prevPos.positionsSize; i++) {
		prevPos.positions[i] = new char[BOARD_SIZE * BOARD_SIZE + 1];
		inFile.read(prevPos.positions[i], BOARD_SIZE * BOARD_SIZE);
		prevPos.positions[i][BOARD_SIZE * BOARD_SIZE] = '\0';
		inFile.read(reinterpret_cast<char*>(&prevPos.positionsCounter[i]), sizeof(int));
	}
	inFile.close();
}

void Game::makeMove()
{
	const int BUFFER_SIZE = 128;
	char moveFrom[BUFFER_SIZE];
	char moveTo[BUFFER_SIZE];
	wcout << (state.whiteMove ? "White" : "Black") << "'s move (e.g. e2 e4): ";
	cin >> moveFrom;
	while (compareStrs(moveFrom, "save", false))
	{
		saveGame();
		wcout << "Game saved\n";
		cin >> moveFrom;
	}
	if (compareStrs(moveFrom, "pause", false))
	{
		wcout << "pausing" << endl;
		return;
	}
	cin >> moveTo;
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
	handlePromotion(rowTo, colTo);
	state.whiteMove = !state.whiteMove;
	savePosition();
}

