#include <iostream>
#include <sstream>
#include <windows.h>
//#include <chrono>
#include "King.h"
#include "Queen.h"
#include "Bishop.h"
#include "Pawn.h"
#include "Knight.h"
#include "Rook.h"

#include "utils.h"
#include "consts.h"
#include "Piece.h"
#include "PreviousPositions.h"

using namespace std;

char toLower(char ch) {
	if (ch >= 'A' && ch <= 'Z') {
		return ch - 'A' + 'a';
	}
	return ch;
}

void setColor(int textColor, int bgColor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, textColor + (bgColor << 4));
}

int getCol(const char* location) {
	return toLower(location[0]) - 'a';
}

int getRow(const char* location) {
	return BOARD_SIZE - (location[1] - '0');
}

void validateLocation(const char* location) {
	if (strlen(location) != 2) {
		throw invalid_argument("Invalid location. Use two characters like 'e2'.");
	}
	char colChar = toLower(location[0]);
	int column = getCol(location);
	if (column < 0 || column >= BOARD_SIZE) {
		throw invalid_argument("Invalid column. Must be a-h.");
	}
	int row = getRow(location);
	if (row < 0 || row >= BOARD_SIZE) {
		throw invalid_argument("Invalid row. Must be 1-8.");
	}
}

int absVal(int number) {
	if (number < 0) {
		return -number;
	}
	return number;
}

bool compareStrs(const char* str1, const char* str2, bool caseSensitive) {
	if (str1 == nullptr || str2 == nullptr) {
		return false;
	}
	while (*str1 && *str2) {
		if (caseSensitive && *str1 != *str2) {
			return false;
		}
		if (!caseSensitive && toLower(*str1) != toLower(*str2)) {
			return false;
		}

		str1++;
		str2++;
	}
	return *str1 == *str2;
}

void fixCin() {
	cin.clear();
	cin.sync();
	cin.ignore();
}

size_t getCurrentTimeInMs() {
	size_t time = GetTickCount64(); // windows
	//size_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(); // cross-platform
	return time;
}

void selectOption(int& option, int from, int to) {
	bool optionSelected = false;
	while (!optionSelected) {
		try {
			cin >> option;
			if (option < from || option > to) {
				ostringstream message;
				message << "Select number between " << from << " and " << to << ":";
				throw invalid_argument(message.str());
			}
			optionSelected = true;
		}
		catch (const exception& e) {
			wcout << e.what() << endl;
			fixCin();
		}
	}
}

Piece* createPiece(COLORS color, PIECES type) {
	switch (type) {
	case KING:   return new King(color);
	case QUEEN:  return new Queen(color);
	case ROOK:   return new Rook(color);
	case BISHOP: return new Bishop(color);
	case KNIGHT: return new Knight(color);
	case PAWN:   return new Pawn(color);
	case NONE:  return new Piece();
	default:
		throw invalid_argument("Unknown piece type in save file.");
	}
}

bool hasPiecesForMate(Piece* board[BOARD_SIZE][BOARD_SIZE]) {
	int blackMinorPieces = 0;
	COLORS blackBishopSquareColor = NONE;
	int whiteMinorPieces = 0;
	COLORS whiteBishopSquareColor = NONE;
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			Piece* piece = board[row][col];
			if (piece->isEmpty() || piece->getType() == KING) {
				continue;
			}
			PIECES type = piece->getType();
			if (type == PAWN || type == ROOK || type == QUEEN) {
				return true;
			}
			COLORS squareColor = (row + col) % 2 == 0 ? WHITE : BLACK;
			if (piece->getColor() == WHITE) {
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
				if (type == KNIGHT || (type == BISHOP && squareColor != blackBishopSquareColor)) {
					blackMinorPieces++;
				}
				if (type == BISHOP) {
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

bool hasThreefoldRepetition(const PreviousPositions& prevPos) {
	for (size_t i = 0; i < prevPos.positionsSize; i++) {
		if (prevPos.positionsCounter[i] >= 3) {
			return true;
		}
	}
	return false;
}

bool hasFiftyMoveRuleHappened(const PreviousPositions& prevPos) {
	int counter = 0;
	for (size_t i = 0; i < prevPos.positionsSize; i++) {
		counter += prevPos.positionsCounter[i];
		if (counter >= 50) {
			return true;
		}
	}
	return false;
}

char* encodeBoard(Piece* board[BOARD_SIZE][BOARD_SIZE]) {
	char* result = new char[BOARD_SIZE * BOARD_SIZE + 1];
	for (size_t row = 0; row < BOARD_SIZE; row++) {
		for (size_t col = 0; col < BOARD_SIZE; col++) {
			size_t index = row * BOARD_SIZE + col;
			Piece* piece = board[row][col];
			char symbol = piece->getType() + 'A';
			result[index] = piece->getColor() == WHITE ? symbol : toLower(symbol);
		}
	}
	result[BOARD_SIZE * BOARD_SIZE] = '\0';
	return result;
}

void pSwap(Piece*& p1, Piece*& p2) {
	Piece* temp = p1;
	p1 = p2;
	p2 = temp;
}