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
	size_t totalTimeInMs = 0;
	size_t timePerMoveInMs = 0;

	wcout << L"Welcome to chess. Select one of the options below: \n(1) New Game \n(2) Load Game \n";
	selectOption(option, 1, 2);

	if (option == 1)
	{
		wcout << L"Select the type of the game: \n(1) No time control \n(2) Timed game \n";
		selectOption(option, 1, 2);
		if (option == 2)
		{
			int time;
			wcout << L"Choose total time per player in minutes(1-300)\n";
			selectOption(time, 1, 300);
			totalTimeInMs = time * 60 * 1000;
			wcout << L"Choose added time per move in seconds(0-300)\n";
			selectOption(time, 0, 300);
			timePerMoveInMs = time * 1000;
			chessGame.setTimeControl(totalTimeInMs, timePerMoveInMs);
		}
	}
	else {
		try {
			chessGame.loadGame();
		}
		catch (const exception& e) {
			wcout << e.what() << endl;
			return 0;
		}
	}
	chessGame.printBoard();
	while (!chessGame.isGameOver()) {
		bool validMove = false;
		while (!validMove) {
			try {
				chessGame.makeMove();
				validMove = true;
			}
			catch (const invalid_argument& e) {
				wcout << e.what() << endl;
			}
			catch (const logic_error& e) {
				wcout << L"Logic error: " << e.what() << endl;
			}
		}
		chessGame.printBoard();
	}
}