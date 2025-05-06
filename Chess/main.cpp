#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

#include "consts.h"
#include "Game.h"

using namespace std;

void printBoard() {

}

int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);

	Game chessGame;
	chessGame.printBoard();
}