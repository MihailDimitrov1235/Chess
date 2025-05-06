#pragma once
#include "Piece.h"

char toLower(char ch);
void setColor(int textColor, int bgColor);
int getCol(const char* location);
int getRow(const char* location);
void validateLocation(const char* location);
void swapPieces(Piece& p1, Piece& p2);