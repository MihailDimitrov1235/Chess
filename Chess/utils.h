#pragma once
#include "Piece.h"

char toLower(char ch);
void setColor(int textColor, int bgColor);
int getCol(const char* location);
int getRow(const char* location);
void validateLocation(const char* location);
int absVal(int number);
bool compareStrs(const char* str1, const char* str2, bool caseSensitive = true);
void fixCin();