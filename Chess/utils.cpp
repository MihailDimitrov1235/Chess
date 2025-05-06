#include <iostream>
#include <windows.h>

#include "utils.h"

using namespace std;

void setColor(int textColor, int bgColor) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, textColor + (bgColor << 4));
}