// BoggleGame.cpp

#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <algorithm>

using namespace std;

const int SIDES_ON_DIE = 6;
const int BOARD_ROWS = 4;
const int BOARD_COLS = 4;
const int NUMBER_OF_SHAKES = 500;

const string BOARD_ROW_SEPARATOR = "------------------";
const string BOARD_COL_SEPARATOR = "|";
const string SPACE = " ";

/*
Takes a 2D array of strings (die) and another array to keep track of which side
of the die is up. The function modifies these parameters according to what is read
from the text file "BoggleDiceCaps.txt". All 'sidesUp' are set to 0 by default.
*/
void loadDice(string[BOARD_ROWS][BOARD_COLS], int sidesUp[BOARD_ROWS][BOARD_COLS]);

/*
Simply prints the board into the console. Does not clear the screen or modify data.
*/
void printBoard(string stringArr[BOARD_ROWS][BOARD_COLS], int sidesUp[BOARD_ROWS][BOARD_COLS]);

/*
Given a board of die, it will swap die randomly and change the top facing sides of die randomly.
It will modify the data 'int shakes' times. Random number generator must be seeded somewhere else.
*/
void shakeDice(string stringArr[BOARD_ROWS][BOARD_COLS], int sidesUp[BOARD_ROWS][BOARD_COLS], int shakes);

/*
Given a word and a Boggle board, it will return whether the given word exists in the board
*/
bool checkWord(string stringArr[BOARD_ROWS][BOARD_COLS], int sidesUp[BOARD_ROWS][BOARD_COLS], string word);

/*
Checks if the first character in 'word' exists at stringArr[rIndex][cIndex].at(sidesUp[rIndex][cIndex]).
If it does, it will trim off the first character and recurse. With each recursion, the 'rIndex' and
'cIndex' will change depending on what direction is given (xDir & yDir).
*/
bool checkDirection(string stringArr[BOARD_ROWS][BOARD_COLS], int sidesUp[BOARD_ROWS][BOARD_COLS], string word, int xDir, int yDir, int rIndex, int cIndex);

/*
Simple function to check if a given index is valid on the Boggle board
*/
bool inBounds(int row, int col);


int main() {

	// the array of dice is the same as the board array, so only one variable is used
	string diceArr[BOARD_ROWS][BOARD_COLS];

	// values correspond to the values in diceArr, used to keep track of which sides are up
	int sidesUp[BOARD_ROWS][BOARD_COLS];

	// stores the word input from the user
	string word;

	srand((unsigned)time(NULL));

	loadDice(diceArr, sidesUp);

	printBoard(diceArr, sidesUp);

	// loops forever, must be stopped manually
	while (true) {

		cout << "\n\nPress Enter to shake the board." << endl;

		// waits for key press
		getchar();

		// shakes di
		for (int i = 0; i < NUMBER_OF_SHAKES; i++) {

			system("cls");
			shakeDice(diceArr, sidesUp, 1);
			printBoard(diceArr, sidesUp);
			
		}


		cout << "\n\n" << "Please enter a word:" << endl;
		cin >> word;

		// makes 'word' all caps so the input isn't case sensitive
		transform(word.begin(), word.end(), word.begin(), ::toupper);

		while (word != "ESCAPE" && !checkWord(diceArr, sidesUp, word)) {

			cout << "That word does not exist." << endl;

			cout << "Please enter another word or enter 'escape' to escape:" << endl;
			cin >> word;

			transform(word.begin(), word.end(), word.begin(), ::toupper);

		}

		if(word != "ESCAPE")
			cout << "That word exists!" << endl;


		// pause
		getchar();
	}

	return 0;
}


bool checkWord(string stringArr[BOARD_ROWS][BOARD_COLS], int sidesUp[BOARD_ROWS][BOARD_COLS], string word) {

	// loop through each row
	for (int y = 0; y < BOARD_ROWS; y++) {

		// loop through each column
		for (int x = 0; x < BOARD_COLS; x++) {

			// if the first character of the word is found on the board
			if (stringArr[y][x].at(sidesUp[y][x]) == word.at(0)) {

				/*

				The parameters of checkDirection() require a direction to look
				for the word. It determines direction from two variables, one for
				rows and one for columns. It checks up, down, right, left, and
				diagonals.


				Direction Value Parameters

				above:         yDir = -1 | xDir = 0
				below:          yDir = 1 | xDir = 0
				right:          yDir = 0 | xDir = 1
				left:           yDir = 0 | xDir = -1
				upper-right:   yDir = -1 | xDir = 1
				lower-right:    yDir = 1 | xDir = 1
				lower-left:     yDir = 1 | xDir = -1
				upper-left:    yDir = -1 | xDir = -1

				*/

				// Check above
				if (checkDirection(stringArr, sidesUp, word, 0, -1, y, x)) return true;

				// Check below
				if (checkDirection(stringArr, sidesUp, word, 0, 1, y, x)) return true;

				// Check right
				if (checkDirection(stringArr, sidesUp, word, 1, 0, y, x)) return true;

				// Check left
				if (checkDirection(stringArr, sidesUp, word, -1, 0, y, x)) return true;

				// Check upper right
				if (checkDirection(stringArr, sidesUp, word, 1, -1, y, x)) return true;

				// Check lower right
				if (checkDirection(stringArr, sidesUp, word, 1, 1, y, x)) return true;

				// Check lower left
				if (checkDirection(stringArr, sidesUp, word, -1, 1, y, x)) return true;

				// Check upper left
				if (checkDirection(stringArr, sidesUp, word, -1, -1, y, x)) return true;

			}
		}
	}

	// if the program reaches here, it means it did not find anything so return false
	return false;
}


bool checkDirection(string stringArr[BOARD_ROWS][BOARD_COLS], int sidesUp[BOARD_ROWS][BOARD_COLS], string word, int xDir, int yDir, int rIndex, int cIndex) {

	// if letter on board equals first letter in 'word'
	if (stringArr[rIndex][cIndex].at(sidesUp[rIndex][cIndex]) == word.at(0)) {

		// if word is only one character, there is no more to test, so return true
		if (word.length() < 2) {
			return true;

			// else if the next index is valid
		}
		else if (inBounds(rIndex + yDir, cIndex + xDir)) {

			// chop off the first character and recurse at next index
			return checkDirection(stringArr, sidesUp, word.substr(1), xDir, yDir, yDir + rIndex, xDir + cIndex);

		}
		else {

			// if it is more than one char and out of bounds, return fales
			return false;

		}

	} // else statement not requried because if statement above is garunteed to return something if true

	  // nothing was found, return false
	return false;
}


bool inBounds(int row, int col) {

	return (row >= 0 && row < BOARD_ROWS) && (col >= 0 && col < BOARD_COLS);

}


void loadDice(string stringArr[BOARD_ROWS][BOARD_COLS], int sidesUp[BOARD_ROWS][BOARD_COLS]) {

	ifstream inFile;
	inFile.open("BoggleDiceCaps.txt");

	// since it doesn't use 2 nested for loops like elsewhere, it uses these variables to keep track
	int row = 0;
	int col = 0;

	string tempString;

	// while not end of file
	while (!inFile.eof()) {

		// try to get the next string
		inFile >> tempString;

		// checks the fail bit
		if (!inFile.fail()) {

			// if row becomes invalid, exit the function
			if (row > BOARD_ROWS - 1) return;

			stringArr[row][col] = tempString;

			// side up is 0 by default
			sidesUp[row][col] = 0;

			// checks if it is on the last column
			if (col == BOARD_COLS - 1) {
				row++;
				col = 0;
			}

			// if it isn't on the last column, it goes to the next one
			else {
				col++;
			}
		}
	}

	inFile.close();

}

void printBoard(string stringArr[BOARD_ROWS][BOARD_COLS], int sidesUp[BOARD_ROWS][BOARD_COLS]) {

	cout << BOARD_ROW_SEPARATOR << endl;

	// loops through all rows
	for (int y = 0; y < BOARD_ROWS; y++) {

		cout << BOARD_COL_SEPARATOR;

		// loops through all columns
		for (int x = 0; x < BOARD_COLS; x++) {

			// if/else statements to check for a Q
			if (stringArr[y][x].at(sidesUp[y][x]) == 'Q') {

				cout << SPACE << stringArr[y][x].at(sidesUp[y][x]) << "u" << SPACE << BOARD_COL_SEPARATOR;

			}
			else {

				cout << SPACE << stringArr[y][x].at(sidesUp[y][x]) << SPACE << BOARD_COL_SEPARATOR;

			}
		}

		cout << endl << BOARD_ROW_SEPARATOR << endl;

	}
}


void shakeDice(string stringArr[BOARD_ROWS][BOARD_COLS], int sidesUp[BOARD_ROWS][BOARD_COLS], int shakes) {


	for (int i = 0; i < shakes; i++) {

		// store the random indices
		int randRow1 = rand() % BOARD_ROWS;
		int randCol1 = rand() % BOARD_COLS;
		int randRow2 = rand() % BOARD_ROWS;
		int randCol2 = rand() % BOARD_COLS;

		// swap dice
		string tempStr = stringArr[randRow1][randCol1];
		stringArr[randRow1][randCol1] = stringArr[randRow2][randCol2];
		stringArr[randRow2][randCol2] = tempStr;

		// randomize sides
		sidesUp[randRow1][randCol1] = rand() % SIDES_ON_DIE;
		sidesUp[randRow2][randCol2] = rand() % SIDES_ON_DIE;

	}
	
}

