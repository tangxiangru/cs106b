/* Implementation of the Boggle Class
 * Creator: Cristobal Sciutto
 * Section Leader: Shirin Salehi
 *
 * Extra Features: support for 5x5 Boggle Board
 * Look at lines: Constructor: 46 - 47, Board: 70 - 80 + Substitution of 4 for size elsewhere;
 *
 * Observation: due to a bug in the boggleGui file, when calling
 * BoggleGUI::close(), the program would always crash. Commenting
 * out line 161 on bogglegui.cpp fixed that.
 */

#include "Boggle.h"
#include "bogglegui.h"
#include "shuffle.h"
#include "strlib.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};


/***************************************************************************
 Functions: Constructor
 ---------------------------------------------------------------------------
 Initiates a Boggle board, setting the instance variables dictionary and
 boardText to the parameters passed in. Furthermore, if the boardText passed
 in is an empty string (this is determined in boggleplay), then a random board
 is created. The grid called board is then updated using the setBoardText method.
 ***************************************************************************/

Boggle::Boggle(Lexicon& dictionary, string boardText, int size) {
    this->size = size;
    board.resize(size, size);

    // In case the boardText passed in is empty, a random one is generated
    if (boardText.empty()) boardText = getRandomBoard();
    this->boardText = boardText;
    // Populating the board grid
    setBoardText();

    this->dictionary = dictionary;
    humanScore = 0;
    computerScore = 0;
}

/***************************************************************************
 Functions: getRandomBoard()
 ---------------------------------------------------------------------------
 Generates a random string of length 16 from the CUBES array which contains
 the cubes of a boggle cube. Randomness is guaranteed by shuffling the array
 and each cube before choosing the letters.
 ***************************************************************************/

string Boggle::getRandomBoard() {
    string boardText = "";
    if (size == 4) {
        shuffle(CUBES, 16);
        for (string cube : CUBES) {
            boardText += cube[randomInteger(0,5)];
        }
    } else {
        shuffle(BIG_BOGGLE_CUBES, 25);
        for (string cube : BIG_BOGGLE_CUBES) {
            boardText += cube[randomInteger(0,5)];
        }
    }

    return toUpperCase(boardText);
}

/***************************************************************************
 Functions: setBoardText()
 ---------------------------------------------------------------------------
 Populates the board Grid<char> with the boardText instance variable by looping
 through the board and using the c + length * r formula to properly align.
 ***************************************************************************/

void Boggle::setBoardText() {
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            int index = c + (size * r);
            board[r][c] = boardText[index];
		}
	}
}

/***************************************************************************
 Functions: getLetter()
 ---------------------------------------------------------------------------
 Checks if the row and col passed in is in bounds and then returns the
 corresponding letter. Else, it throws an int exception.
 ***************************************************************************/

char Boggle::getLetter(int row, int col) {
	if (board.inBounds(row, col)) {
        return board.get(row, col);
	} else throw 20;
}

/***************************************************************************
 Functions: checkWord()
 ---------------------------------------------------------------------------
 Checks if a word satisfies the conditions to be in a boggle game, namely:
 (1) Hasn't been found yet (2) Is in the dictionary (3) Atleast 4 letters long
 Returns true is all conditions are satisfied
 Additionally checks to make sure the word > 16 char.
 ***************************************************************************/

bool Boggle::checkWord(string word) {
    word = toUpperCase(word);
    if (!wordsFound.contains(word) &&
            dictionary.contains(word) &&
            word.length() >= 4 &&
            (int) word.length() <= size*size) {
        return true;
    }
	else return false;
}

/***************************************************************************
 Functions: humanWordSearch and helper function
 ---------------------------------------------------------------------------
 The humanWordSearch loops through the grid looking for the first letter of
 the word passed in. It then calls upon the humanHelper function to see if
 the word can be found. In order to do this, a Grid<bool> is passed in, which
 will be used to avoid repetitions.

 The humanHelper loops around the characters adjacent to the position passed
 in, looking for the next char in the word. It then recursively calls itself
 to look for the next letter. The base case is when the word passed in is empty,
 and therefore the full word has been found. In that case, true is returned.
 Else, the recursion backtracks and continues with the next possible path.
 If no paths lead to the word, false is returned.
 ***************************************************************************/

bool Boggle::humanHelper(string word, Grid<bool> & checked, int r, int c) {
    if (word.empty()) return true;
    // Loop through adjacent cubes
    for (int dr = -1; dr < 2; dr++) {
        for (int dc = -1; dc < 2; dc++) {
            if (board.inBounds(r+dr,c+dc) &&
                    board[r+dr][c+dc] == word[0] &&
                    checked[r+dr][c+dc] != 1) {
                // Recursion Set Up
                checked[r+dr][c+dc] = true;
                BoggleGUI::setAnimationDelay(100);
                BoggleGUI::setHighlighted(r+dr, c+dc, true);
                // Recursive Step;
                if (humanHelper(word.substr(1), checked, r+dr, c+dc)) return true;
                // Backtracking Undo
                checked[r+dr][c+dc] = false;
                BoggleGUI::setHighlighted(r+dr, c+dc, false);
            }
        }
    }
    return false;
}

bool Boggle::humanWordSearch(string word) {
    Grid<bool> checked(size, size);
    checked.fill(false);
    // Loop through board
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            if (board[r][c] == word[0]) {
                if (humanHelper(word, checked, r, c)) {
                    wordsFound.add(word);
                    return true;
                }
            }
        }
    }
    return false;
}

/* getWords(): is a getter function for the wordsFound variable */

Set<string> Boggle::getWords() {
    return wordsFound;
}

/* getScoreHuman(): is a getter function for the humanScore variable */

int Boggle::getScoreHuman() {
    return humanScore;
}

/* addScoreHuman(): is a pseudo-setter function for the humanScore
   variable, incrementing its value by n
*/

void Boggle::addScoreHuman(int n) {
    humanScore += n;
}

/***************************************************************************
 Functions: computerWordSearch and helper function
 ---------------------------------------------------------------------------
 In order to find all of the remainding words on a boggle board, the computer
 WordSearch function loops through each cube on the board, and calls a helper
 function to find words starting at that letter. To mark cubes already used,
 a Grid<bool> is created. Furthermore, a Set<string> called result is used to
 store words found, and a string called word is used to build up words recur-
 sively and check if they exists.

 The helper function's base case is to check if the word is in the dictionary,
 hasn't been found yet, and is longer than 3 characters. In that case it is
 added to the result set passed in by reference. Then, if the word is a prefix
 of any other word in the dictionary, the helper loops through adjacent cubes
 and is called recursively on each one, as long as it hasn't been used before.
 Recursive backtracking is used on both the Grid<bool> and the word that is
 being built up in order to properly have all possibilities.
 ***************************************************************************/

void Boggle::computerHelper(Set<string> & result, Grid<bool> & checked, string word, int r, int c) {
    if (dictionary.contains(word) &&
            !wordsFound.contains(word) &&
            word.length() > 3)
        result.add(word);

    if (dictionary.containsPrefix(word)) {
        // Loop through adjacent cubes
        for (int dr = -1; dr < 2; dr++) {
            for (int dc = -1; dc < 2; dc++) {
                if (board.inBounds(r+dr,c+dc) && checked[r+dr][c+dc] != 1) {
                    // Recursive Updates
                    checked[r+dr][c+dc] = true;
                    word += board[r+dr][c+dc];
                    // Recursive Call
                    computerHelper(result, checked, word, r+dr, c+dc);
                    // Recursive Backtracking
                    checked[r+dr][c+dc] = false;
                    word.pop_back();
                }
            }
        }
    }
}

Set<string> Boggle::computerWordSearch() {
    Set<string> result;
    string word;
    Grid<bool> checked(size, size);
    // Loop through board
    for (int r = 0; r < size; r++) {
        for (int c = 0; c < size; c++) {
            // Recursive Setup and Call
            word += board[r][c];
            checked[r][c] = true;
            computerHelper(result, checked, word, r, c);
            // Recursive Backtrack
            checked[r][c] = false;
            word.pop_back();
        }
    }
    return result;
}

/* getScoreComputer(): is a getter function for the computerScore variable */

int Boggle::getScoreComputer() {
    return computerScore;
}

/* addScoreComputer(): is a pseudo-setter function for the computerScore
   variable, incrementing its value by n
*/

void Boggle::addScoreComputer(int n) {
    computerScore += n;
}

/* getText(): is a getter function for the boardText variable */

string Boggle::getText() {
    return boardText;
}

/***************************************************************************
 Functions: operator <<
 ---------------------------------------------------------------------------
 Prints out the boardText into a 4x4 grid shape by looping twice upto 4 and
 using the (c + 4 * r) formula to correctly index the string. After each row
 loop, and endl is printed to continue on the next line.
 ***************************************************************************/

ostream& operator<<(ostream& out, Boggle & boggle) {
    for (int r = 0; r < boggle.size; r++) {
        for (int c = 0; c < boggle.size; c++) {
            out << boggle.boardText[c + boggle.size * r];
        }
        out << endl;
    }
    return out;
}
