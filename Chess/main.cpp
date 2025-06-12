#include <iostream>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
//#include <cstdlib>
//#include <crtdbg.h>
#include "Game.h"

using namespace std;

int main() {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_setmode(_fileno(stdout), _O_U16TEXT);

	Game chessGame;
	chessGame.startGame();
}