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
bool hasPiecesForMate(const Piece* board[BOARD_SIZE][BOARD_SIZE]);
bool hasThreefoldRepetition(const PreviousPositions& prevPos);
bool hasFiftyMoveRuleHappened(const PreviousPositions& prevPos);
char* encodeBoard(const Piece* board[BOARD_SIZE][BOARD_SIZE]);
void pSwap(Piece*& p1, Piece*& p2);