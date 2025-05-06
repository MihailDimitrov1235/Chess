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
	while (!chessGame.isGameOver()) {
		chessGame.printBoard();
		try {
			chessGame.makeMove();
		}
		catch (invalid_argument e) {
			wcout << e.what() << endl;
		}
	}
}