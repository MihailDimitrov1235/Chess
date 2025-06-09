#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>

#include "consts.h"
#include "Game.h"
#include "utils.h"

using namespace std;

int main() {
	_setmode(_fileno(stdout), _O_U16TEXT);
	Game chessGame;
	chessGame.startGame();
}