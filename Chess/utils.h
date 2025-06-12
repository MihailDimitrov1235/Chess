#pragma once
#include "Piece.h"
#include "PreviousPositions.h"

char toLower(char ch);
void setColor(int textColor, int bgColor);
int getCol(const char* location);
int getRow(const char* location);
void validateLocation(const char* location);
int absVal(int number);
bool compareStrs(const char* str1, const char* str2, bool caseSensitive = true);
void fixCin();
size_t getCurrentTimeInMs();
void selectOption(int& option, int from, int to);
Piece* createPiece(COLORS color, PIECES type);
bool hasPiecesForMate(Piece* board[BOARD_SIZE][BOARD_SIZE]);
bool hasThreefoldRepetition(PreviousPositions prevPos);
bool hasFiftyMoveRuleHappened(PreviousPositions prevPos);
char* encodeBoard(Piece* board[BOARD_SIZE][BOARD_SIZE]);