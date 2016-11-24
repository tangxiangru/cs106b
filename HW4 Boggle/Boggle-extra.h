/* Header file for Boggle class implementation
 * Creator: Cristobal Sciutto
 * Section Leader: Shirin Salehi
 *
 * Extra Features: support for 5x5 Boggle Board
 * Look at lines: 25, 55
 *
 * Observation: due to a bug in the boggleGui file, when calling
 * BoggleGUI::close(), the program would always crash. Commenting
 * out line 161 on bogglegui.cpp fixed that.
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
#include "set.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "", int size = 4);
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();

    friend ostream& operator<<(ostream& out, Boggle& boggle);

    // My Public Methods
    // Getter for boardText variable
    string getText();
    // Getter for wordsFounds variable
    Set<string> getWords();
    // Updater for computerScore variable
    void addScoreComputer(int n);
    // Updater for humanScore variable
    void addScoreHuman(int n);

private:
	// Member Variables
    Lexicon dictionary;
    // Grid<char> is used to store the board
    Grid<char> board;
    // Set<string> is used to store the words found by the human
    Set<string> wordsFound;
    /* String is used as the boardText that is either randomly generated
       or created by user. It is used to populate the Grid<char> */
    string boardText;
    int size;
    int humanScore;
	int computerScore;

	// Private Methods
    // Helper function for humanWordSearch()
    bool humanHelper(string word, Grid<bool> & checked, int r, int c);
    // Helper function for computerWordSearch()
    void computerHelper(Set<string> & result, Grid<bool> & checked, string word, int r, int c);
    // Generates a random boardText from the CUBE arrays
    string getRandomBoard();
    // Populates board with boardText
	void setBoardText();
};

#endif // _boggle_h
