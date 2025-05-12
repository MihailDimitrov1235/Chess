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
	int option;
	bool optionSelected = false;
	bool error = false;
	wcout << "Welcome to chess. Select one of the options below: \n(1) New Game \n(2) Load Game \n";
	while (!optionSelected) {
		try {
			cin >> option;
			if (option < 1 || option > 2)
			{
				throw invalid_argument("Select number between 1 and 2:");
			}
			optionSelected = true;
		}
		catch (const exception& e) {
			wcout << e.what() << endl;
			fixCin();
		}
	}
	if (option == 1)
	{

	}
	else {
		try {
			chessGame.loadGame();
		}
		catch (const exception& e) {
			wcout << e.what() << endl;
			error = true;
		}
	}
	if (error)
	{
		return 0;
	}
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