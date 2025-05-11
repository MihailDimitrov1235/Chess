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
	chessGame.printBoard();
	while (!chessGame.isGameOver()) {
		bool validMove = false;
		while (!validMove) {
			try {
				chessGame.makeMove();
				validMove = true;
			}
			catch (invalid_argument e) {
				wcout << e.what() << endl;
			}
		}
		chessGame.printBoard();
	}
}