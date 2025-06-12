#include <fstream>
#include <iostream>
#include "DataManager.h"
#include "utils.h"

using namespace std;

DataManager::DataManager(Piece* (&board)[BOARD_SIZE][BOARD_SIZE], GameState& state, PreviousPositions& prevPos) : board(board), state(state), prevPos(prevPos) {}

void DataManager::saveGame() const {
	ofstream outFile(fileName, ios::binary);
	if (!outFile) {
		throw runtime_error("Could not open save file.");
	}
	outFile.write(reinterpret_cast<const char*>(&state), sizeof(GameState));

	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		for (size_t j = 0; j < BOARD_SIZE; j++)
		{
			Piece* piece = board[i][j];
			PIECES type = piece->getType();
			COLORS color = piece->getColor();
			outFile.write(reinterpret_cast<const char*>(&type), sizeof(int));
			outFile.write(reinterpret_cast<const char*>(&color), sizeof(int));

		}
	}

	outFile.write(reinterpret_cast<const char*>(&prevPos.positionsSize), sizeof(int));
	for (int i = 0; i < prevPos.positionsSize; i++) {
		outFile.write(prevPos.positions[i], BOARD_SIZE * BOARD_SIZE);
		outFile.write(reinterpret_cast<const char*>(&prevPos.positionsCounter[i]), sizeof(int));
	}
	outFile.close();
}

void DataManager::loadGame()
{
	ifstream inFile(fileName, ios::binary);
	if (!inFile) {
		throw runtime_error("Could not open save file.");
	}
	inFile.read(reinterpret_cast<char*>(&state), sizeof(GameState));

	for (size_t i = 0; i < BOARD_SIZE; i++)
	{
		for (size_t j = 0; j < BOARD_SIZE; j++)
		{
			PIECES type;
			COLORS color;
			inFile.read(reinterpret_cast<char*>(&type), sizeof(int));
			inFile.read(reinterpret_cast<char*>(&color), sizeof(int));
			delete board[i][j];
			board[i][j] = createPiece(color, type);
		}
	}

	inFile.read(reinterpret_cast<char*>(&prevPos.positionsSize), sizeof(int));
	prevPos.positions = new char* [prevPos.positionsSize];
	prevPos.positionsCounter = new int[prevPos.positionsSize];
	for (int i = 0; i < prevPos.positionsSize; i++) {
		prevPos.positions[i] = new char[BOARD_SIZE * BOARD_SIZE + 1];
		inFile.read(prevPos.positions[i], BOARD_SIZE * BOARD_SIZE);
		prevPos.positions[i][BOARD_SIZE * BOARD_SIZE] = '\0';
		inFile.read(reinterpret_cast<char*>(&prevPos.positionsCounter[i]), sizeof(int));
	}
	state.paused = true;
	inFile.close();
}