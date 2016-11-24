/* Boggle Play file: determines CLI for game
 * Creator: Cristobal Sciutto
 * Section Leader: Shirin Salehi
 */

#include "lexicon.h"
#include "console.h"
#include "simpio.h"
#include "strlib.h"
#include "Boggle.h"
#include "bogglegui.h"

using namespace std;

/***************************************************************************
 Functions: checkInput() and inputForBoard()
 ---------------------------------------------------------------------------
 This pair of functions requests the user for a string of 16 characters to be
 used as the initial grid for a boggle game. The first function makes sure the
 string is of proper size and only letters. The second is responsible for
 requesting multiple times until the conditions are satisfied.
 ***************************************************************************/

bool checkInput(string boardText) {
    if (boardText.size() != 16) return false;
	for (char c : boardText) {
        if (!isalpha(c)) return false;
	}
	return true;
}

string inputForBoard() {
    string boardText;
	while (true) {
        boardText = getLine("Type the 16 letters to appear on the board: ");
		if (checkInput(boardText)) break;
		else cout << "That is not a valid 16-letter board string. Try again." << endl;
	}
    // Always return an uppercase board text as a standard
	return toUpperCase(boardText);
}

/***************************************************************************
 Functions: printRoundIntro() and playOneRound()
 ---------------------------------------------------------------------------
 These functions compose a round of the human-phase in the boggle game.
 printRoundIntro takes care of updating the user on the games current situation
 while playOneRound ask the user for a word and then verifies if it's valid,
 adding it to the list and increasing the human's score. All of this is done
 through the Boggle class interface.

 playOneRound returns a string which is essentially the result from that round.
 That string is then used in the intro of the following round.
 ***************************************************************************/

void printRoundIntro(string prompt, Boggle & myBoard) {
	clearConsole();
	cout << prompt << endl;
    cout << myBoard << endl;
    cout << "Your words (" << myBoard.getWords().size() << "): ";
    cout << myBoard.getWords() << endl;
    cout << "Your score: " << myBoard.getScoreHuman() << endl;
}

string playOneRound(Boggle & myBoard) {
    // Once the next word is typed in, clear the highlighting from the previoues
    string word = toUpperCase(getLine("Type a word (or Enter to stop): "));
    BoggleGUI::clearHighlighting();

    // Case the input is just "Enter", break
    if (word.empty()) {
        return "";
    }
    // Make sure the word is "legal"
    else if (!myBoard.checkWord(word)) {
        return "You must enter an unfound 4+ letter word from the dictionary.";
    }
    // If word can be found, update score and GUI
    else if (myBoard.humanWordSearch(word)) {
        myBoard.addScoreHuman(word.length() - 3);
        BoggleGUI::setScore(word.length() - 3, BoggleGUI::HUMAN);
        BoggleGUI::recordWord(word, BoggleGUI::HUMAN);
		return "You have found a new word! \"" + word + "\"";
    }
    // Else, return that the word can't be found
    else {
        return "That word can't be formed on this board.";
    }
}

/***************************************************************************
 Functions: computerUpdate()
 ---------------------------------------------------------------------------
 Givern a set of words found by the computer, using the Boggle interface,
 this function updates the computer's score with each word, and records it
 to the GUI interface. It then set's the score on the GUI and prints out
 the final statistics for the computers play.
 ***************************************************************************/

void computerUpdate(Boggle & myBoard, Set<string> result) {
    for (string word : result) {
        myBoard.addScoreComputer(word.length() - 3);
        BoggleGUI::recordWord(word, BoggleGUI::COMPUTER);
    }
    BoggleGUI::setScore(myBoard.getScoreComputer(), BoggleGUI::COMPUTER);

    cout << "My words (" << result.size() << "): " << result << endl;
    cout << "My score: " << myBoard.getScoreComputer() << endl;
}

/***************************************************************************
 Functions: computerRound
 ---------------------------------------------------------------------------
 Enacts the computer simulation by calling computerWordSearch via the Boggle
 interface, and then updates scores by calling computerUpdate
 ***************************************************************************/

void computerRound(Boggle & myBoard) {
    cout << endl << "It's my turn!" << endl;
    // "Computer" finds words left and then callss computerUpdate()
    Set<string> computerWords = myBoard.computerWordSearch();
    computerUpdate(myBoard, computerWords);
}

/***************************************************************************
 Functions: printWinner
 ---------------------------------------------------------------------------
 Determines winner of by comparing scores and printing different frases in
 case the Human or Computer wins.
 ***************************************************************************/

void printWinner(Boggle & myBoard) {
    if (myBoard.getScoreComputer() > myBoard.getScoreHuman()) {
        string result = "Ha ha ha, I destroyed you. Better luck next time, puny human!";
        cout << result << endl;
        BoggleGUI::setStatusMessage(result);
    } else {
        string result = "WOW, you defeated me! Congratulations!";
        cout << result  << endl;
        BoggleGUI::setStatusMessage(result);
    }
}

/***************************************************************************
 Functions: playOneGame()
 ---------------------------------------------------------------------------
 This is the main function which is used in bogglemain to run a single game
 of boggle. It either ask the user for a board to start, or generates a random
 board using the boggle class. It then loops infinitely through the rounds
 until the user requests to leave. Finally, the computer's round is exectued
 and a winner is decided through computerRound() and printWinner().
 ***************************************************************************/

void playOneGame(Lexicon& dictionary) {
    string boardText = getYesOrNo("Do you want to generate a random board? ") ? "" : inputForBoard();
    Boggle myBoard(dictionary, boardText);
	string prompt = "It's your turn!";

    // Setting up the GUI
    BoggleGUI::initialize(4, 4);
    BoggleGUI::labelAllCubes(myBoard.getText());

    // Infinite Round Loop
	while (true) {
        BoggleGUI::setStatusMessage(prompt);
        printRoundIntro(prompt, myBoard);
		prompt = playOneRound(myBoard);
		if (prompt == "") break;
	}

    computerRound(myBoard);
    printWinner(myBoard);
}
