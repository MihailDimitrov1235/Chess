#include <iostream>
#include <sstream>
#include <windows.h>
#include <chrono>

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

int absVal(int number) {
	if (number < 0)
	{
		return -number;
	}
	return number;
}

bool compareStrs(const char* str1, const char* str2, bool caseSensitive)
{
	if (str1 == nullptr || str2 == nullptr) {
		return false;
	}
	while (*str1 && *str2) {
		if (caseSensitive && *str1 != *str2)
		{
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

void fixCin()
{
	cin.clear();
	cin.sync();
	cin.ignore();
}

size_t getCurrentTimeInMs()
{
	size_t time = GetTickCount64(); // windows
	//size_t time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count(); // cross-platform
	return time;
}

void selectOption(int& option, int from, int to)
{
	bool optionSelected = false;
	while (!optionSelected) {
		try {
			cin >> option;
			if (option < from || option > to)
			{
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
