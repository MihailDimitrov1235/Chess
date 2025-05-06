#include <iostream>
#include <windows.h>

#include "utils.h"
#include "consts.h"
#include "Piece.h"

using namespace std;

char toLower(char ch) {
	if (ch >= 'A' && ch <= 'Z')
	{
		return ch - 'A' + 'a';
	}
	return ch;
}

void setColor(int textColor, int bgColor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, textColor + (bgColor << 4));
}

int getCol(const char* location)
{
	return toLower(location[0]) - 'a';
}

int getRow(const char* location)
{
	return BOARD_SIZE - (location[1] - '0');
}

void validateLocation(const char* location)
{
	if (strlen(location) != 2)
	{
		throw invalid_argument("Invalid location. Use two characters like 'e2'.");
	}
	char colChar = toLower(location[0]);
	int column = getCol(location);
	if (column < 0 || column >= BOARD_SIZE)
	{
		throw invalid_argument("Invalid column. Must be a-h.");
	}
	int row = getRow(location);
	if (row < 0 || row >= BOARD_SIZE)
	{
		throw invalid_argument("Invalid row. Must be 1-8.");
	}
}

void movePiece(Piece& p1, Piece& p2) {
	p2 = p1;
	p1 = Piece();
}