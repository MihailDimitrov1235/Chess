#include <iostream>
#include <fstream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include "Game.h"
#include "consts.h"
#include "utils.h"
#include "Rook.h"
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Knight.h"
#include "Pawn.h"


using namespace std;

void Game::setupBackRank(COLORS color, int row) {
	board[row][0] = new Rook(color);
	board[row][1] = new Knight(color);
	board[row][2] = new Bishop(color);
	board[row][3] = new Queen(color);
	board[row][4] = new King(color);
	board[row][5] = new Bishop(color);
	board[row][6] = new Knight(color);
	board[row][7] = new Rook(color);
}

void Game::setupPawns(COLORS color, int row) {
	for (size_t col = 0; col < BOARD_SIZE; col++) {
		board[row][col] = new Pawn(color);
	}
}

void Game::freePositionsMemory() {
	for (size_t i = 0; i < prevPos.positionsSize; i++)
	{
		delete[] prevPos.positions[i];
	}
	delete[] prevPos.positions;
	prevPos.positions = nullptr;
	delete[] prevPos.positionsCounter;
	prevPos.positionsCounter = nullptr;
	prevPos.positionsSize = 0;
}

void Game::free() {
	freePositionsMemory();
	for (size_t i = 0; i < BOARD_SIZE; i++) {
		for (size_t j = 0; j < BOARD_SIZE; j++) {
			delete board[i][j];
		}
	}
}

void Game::copyFrom(const Game& other) {
	for (size_t row = 0; row < BOARD_SIZE; row++) {
		for (size_t col = 0; col < BOARD_SIZE; col++) {
			if (other.board[row][col] != nullptr) {
				board[row][col] = other.board[row][col]->clone();
			}
			else {
				board[row][col] = nullptr;
			}
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

		for (int i = 0; i < prevPos.positionsSize; i++) {
			prevPos.positions[i] = new char[BOARD_SIZE * BOARD_SIZE + 1];
			strcpy_s(prevPos.positions[i], BOARD_SIZE * BOARD_SIZE + 1, other.prevPos.positions[i]);
			prevPos.positionsCounter[i] = other.prevPos.positionsCounter[i];
		}
	}
	else {
		prevPos.positions = nullptr;
		prevPos.positionsCounter = nullptr;
	}

	state.paused = true;
}

void Game::savePosition() {
	char* newPosition = encodeBoard(board);
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

Game::Game() : validator(board, state), dataManager(board, state, prevPos) {

	for (int row = 2; row < BOARD_SIZE - 2; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			board[row][col] = new Piece();
		}
	}

	setupBackRank(BLACK, 0);
	setupPawns(BLACK, 1);
	setupPawns(WHITE, BOARD_SIZE - 2);
	setupBackRank(WHITE, BOARD_SIZE - 1);

	savePosition();
}

Game::Game(const Game& other) : validator(board, state), dataManager(board, state, prevPos) {
	copyFrom(other);
}

Game& Game::operator=(const Game& other) {
	if (this != &other) {
		free();
		copyFrom(other);
	}
	return *this;
}

Game::~Game() {
	free();
}

void Game::chooseGamemode() {
	int option;
	size_t totalTimeInMs = 0;
	size_t timePerMoveInMs = 0;

	wcout << L"Welcome to chess. Select one of the options below: \n(1) New Game \n(2) Load Game \n";
	selectOption(option, 1, 2);

	if (option == 1)
	{
		wcout << L"Select the type of the game: \n(1) No time control \n(2) Timed game \n";
		selectOption(option, 1, 2);
		if (option == 2)
		{
			int time;
			wcout << L"Choose total time per player in minutes(1-300)\n";
			selectOption(time, 1, 300);
			totalTimeInMs = time * 60 * 1000;
			wcout << L"Choose added time per move in seconds(0-300)\n";
			selectOption(time, 0, 300);
			timePerMoveInMs = time * 1000;
			setTimeControl(totalTimeInMs, timePerMoveInMs);
		}
	}
	else {
		try {
			dataManager.loadGame();
		}
		catch (const exception& e) {
			wcout << e.what() << endl;
			return;
		}
	}
}

void Game::printCols(bool reverse) const {
	wcout << L"  ";
	for (size_t i = 0; i < BOARD_SIZE; i++) {
		wchar_t letter = (reverse ? L'a' + BOARD_SIZE - i - 1 : L'a' + i);
		wcout << letter << L" ";
	}
	wcout << endl;
}

void Game::printBoard() const {
	wcout << L"\033[2J\033[H"; // clear console and start top left

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

			Piece* piece = reverse ? board[BOARD_SIZE - i - 1][BOARD_SIZE - j - 1] : board[i][j];
			wcout << piece->getCode();

			if (j != BOARD_SIZE - 1 || piece->getType() == EMPTY_SQUARE)
			{
				wcout << L" ";
			}
		}
		setColor(WHITE_COLOR, BLACK_COLOR);
		if (board[i][BOARD_SIZE - 1]->getType() != EMPTY_SQUARE) {
			wcout << L" ";
		}
		wcout << L" " << (reverse ? i + 1 : BOARD_SIZE - i) << endl;
	}

	printCols(reverse);

	if (state.timedGame)
	{
		size_t timeLeft = state.whiteMove ? state.whiteTimeInMs : state.blackTimeInMs;
		wcout << L"Time left: " << timeLeft / 1000 / 60 << L":" << timeLeft / 1000 % 60 << (state.paused ? L" PAUSED" : L"") << endl;
	}
	wcout << (state.whiteMove ? L"White" : L"Black") << L"'s move (e.g. e2 e4): ";
}

bool Game::doesPieceHaveLegalMoves(int row, int col) {
	Piece* piece = board[row][col];
	int** moves = piece->getMoves();

	for (int i = 0; i < piece->getMovesSize(); i++) {
		if (moves[i][0] == 0 && moves[i][1] == 0) {
			continue;
		}
		int r = row + moves[i][0], c = col + moves[i][1];
		while (r < BOARD_SIZE && r >= 0 && c < BOARD_SIZE && c >= 0) {
			try {
				validator.validateMove(row, col, r, c);
				return true;
			}
			catch (const invalid_argument&) {}
			if (!piece->getSliding()) {
				break;
			}
			r += moves[i][0];
			c += moves[i][1];
		}
	}
	return false;
}

bool Game::doesPlayerHaveLegalMoves() {
	COLORS color = state.whiteMove ? WHITE : BLACK;
	for (int row = 0; row < BOARD_SIZE; row++)
	{
		for (int col = 0; col < BOARD_SIZE; col++)
		{
			Piece* currentPiece = board[row][col];
			if (currentPiece->getColor() != color)
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

bool Game::isGameOver() {
	if (!doesPlayerHaveLegalMoves())
	{
		int kingRow = state.whiteMove ? state.whiteKingRow : state.blackKingRow;
		int kingCol = state.whiteMove ? state.whiteKingCol : state.blackKingCol;
		if (validator.isKingCapturableAt(kingRow, kingCol)) {
			wcout << (state.whiteMove ? L"Black" : L"White") << L" wins!";
		}
		else {
			wcout << L"Game ends in a draw because " << (state.whiteMove ? L"White" : L"Black") << L" has no legal moves.";
		}
		return true;
	}
	if (!hasPiecesForMate(board)) {
		wcout << L"Game ends in a draw because of lack of pieces for checkmate.";
		return true;
	}
	if (hasThreefoldRepetition(prevPos))
	{
		wcout << L"Game ends in a draw due to threefold repetition.";
		return true;
	}
	if (state.timedGame && (state.whiteTimeInMs <= 0 || state.blackTimeInMs <= 0))
	{
		wcout << (state.whiteTimeInMs <= 0 ? L"Black" : L"White") << L" wins due to timeout!";
		return true;
	}
	if (hasFiftyMoveRuleHappened(prevPos)) {
		wcout << L"Game ends in a draw due to fifty-move rule.";
		return true;
	}
	return false;
}

void Game::handleCastlingRelatedMove(int rowFrom, int colFrom, int rowTo, int colTo) {
	Piece* movingPiece = board[rowFrom][colFrom];
	PIECES type = movingPiece->getType();
	COLORS color = movingPiece->getColor();
	if (type == KING) {
		if (color == WHITE) {
			state.whiteCanCastleShort = false;
			state.whiteCanCastleLong = false;

			state.whiteKingRow = rowTo;
			state.whiteKingCol = colTo;

			if (rowFrom == 7 && colFrom == 4 && rowTo == 7 && colTo == 6) {
				pSwap(board[7][5], board[7][7]);
			}
			if (rowFrom == 7 && colFrom == 4 && rowTo == 7 && colTo == 2) {
				pSwap(board[7][3], board[7][0]);
			}
		}
		else {
			state.blackCanCastleShort = false;
			state.blackCanCastleLong = false;

			state.blackKingRow = rowTo;
			state.blackKingCol = colTo;

			if (rowFrom == 0 && colFrom == 4 && rowTo == 0 && colTo == 6) {
				pSwap(board[0][5], board[0][7]);
			}
			if (rowFrom == 0 && colFrom == 4 && rowTo == 0 && colTo == 2) {
				pSwap(board[0][3], board[0][0]);
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

	if (board[rowTo][colTo]->getType() == ROOK) {
		if (board[rowTo][colTo]->getColor() == WHITE) {
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
	if (board[rowFrom][colFrom]->getType() == PAWN && rowTo == state.enPassantSquare[0] && colTo == state.enPassantSquare[1]) {
		delete board[rowFrom][colTo];
		board[rowFrom][colTo] = new Piece();
	}

	if (board[rowFrom][colFrom]->getType() == PAWN && absVal(rowTo - rowFrom) == 2) {
		state.enPassantSquare[0] = (rowFrom + rowTo) / 2;
		state.enPassantSquare[1] = colFrom;
	}
	else {
		state.enPassantSquare[0] = -1;
		state.enPassantSquare[1] = -1;
	}
}

void Game::handlePromotion(int row, int col) {
	int promotionRow = state.whiteMove ? 0 : 7;
	COLORS color = state.whiteMove ? WHITE : BLACK;
	if (board[row][col]->getType() == PAWN && promotionRow == row)
	{
		wcout << L"Promote pawn into: \n(1) Queen \n(2) Rook \n(3) Bishop \n(4) Knight\n";
		int selectedPromotion;
		selectOption(selectedPromotion, 1, 4);
		delete board[row][col];
		switch (selectedPromotion)
		{
		case 1:
			board[row][col] = new Queen(color);
			break;
		case 2:
			board[row][col] = new Rook(color);
			break;
		case 3:
			board[row][col] = new Bishop(color);
			break;
		case 4:
			board[row][col] = new Knight(color);
			break;
		default:
			throw logic_error("Invalid promotion piece selected.");
			break;
		}
	}
}

void Game::handleTimeControl() {
	if (!state.timedGame) {
		return;
	}

	size_t currentTime = getCurrentTimeInMs();
	if (state.paused) {
		state.paused = false;
		lastMoveTimestamp = currentTime;
		return;
	}
	size_t timeSpentInMs = currentTime - lastMoveTimestamp;
	if (state.whiteMove) {
		if (state.whiteTimeInMs < timeSpentInMs) {
			state.whiteTimeInMs = 0;
		}
		else {
			state.whiteTimeInMs -= timeSpentInMs;
			state.whiteTimeInMs += state.incrementInMs;
		}
	}
	else {
		if (state.blackTimeInMs < timeSpentInMs) {
			state.blackTimeInMs = 0;
		}
		else {
			state.blackTimeInMs -= timeSpentInMs;
			state.blackTimeInMs += state.incrementInMs;
		}
	}

	lastMoveTimestamp = currentTime;
}

void Game::startGame() {
	chooseGamemode();
	printBoard();
	while (!isGameOver()) {
		bool validMove = false;
		while (!validMove) {
			try {
				makeMove();
				validMove = true;
			}
			catch (const invalid_argument& e) {
				wcout << e.what() << endl;
			}
			catch (const logic_error& e) {
				wcout << L"Logic error: " << e.what() << endl;
			}
		}
		printBoard();
	}
}

void Game::makeMove() {
	const int BUFFER_SIZE = 128;
	char moveFrom[BUFFER_SIZE];
	char moveTo[BUFFER_SIZE];
	cin >> moveFrom;
	while (compareStrs(moveFrom, "save", false))
	{
		dataManager.saveGame();
		wcout << L"Game saved\n";
		cin >> moveFrom;
	}
	if (compareStrs(moveFrom, "pause", false))
	{
		state.paused = true;
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

	if (!board[rowTo][colTo]->isEmpty() || board[rowFrom][colFrom]->getType() == PAWN) {
		freePositionsMemory();
	}

	handleCastlingRelatedMove(rowFrom, colFrom, rowTo, colTo);
	handleEnPassantRelatedMove(rowFrom, colFrom, rowTo, colTo);
	delete board[rowTo][colTo];
	board[rowTo][colTo] = board[rowFrom][colFrom];
	board[rowFrom][colFrom] = new Piece();
	handlePromotion(rowTo, colTo);
	handleTimeControl();
	state.whiteMove = !state.whiteMove;
	savePosition();
}

void Game::setTimeControl(size_t totalTimeInMs, size_t timePerMoveInMs) {
	state.timedGame = true;
	state.whiteTimeInMs = totalTimeInMs;
	state.blackTimeInMs = totalTimeInMs;
	state.incrementInMs = timePerMoveInMs;
	state.paused = true;
}

