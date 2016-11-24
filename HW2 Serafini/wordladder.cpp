// This is my Word Ladder Program
// by Cristobal Sciutto (06099425) 

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "lexicon.h"
#include "stack.h"
#include "queue.h"
#include "simpio.h"
#include "filelib.h"

using namespace std;

// Function Declarations
void printIntro();
string getFileString(string prompt);
void formatWord(string &word);
bool checkWords(string word1, string word2, Lexicon myLex);
Set<string> getNextWords(string word);
Stack<string> getLadder(string word1, string word2, Lexicon myLex);
void printResult(string word1, string word2, Stack<string> ladder);

int main() {
	printIntro();

    string dictFile = getFileString("Dictionary file name? ");
    Lexicon myLex = Lexicon(dictFile);

    while (true) {
        cout << endl;

        string word1 = getLine("Word #1 (or Enter to quit): ");
        if (word1.empty()) { break; };
        formatWord(word1);

        string word2 = getLine("Word #2 (or Enter to quit): ");
        if (word2.empty()) { break; };
        formatWord(word2);

        if (!checkWords(word1, word2, myLex)) { continue; };

        Stack<string> ladder = getLadder(word1, word2, myLex);
        printResult(word1, word2, ladder);
	}

    cout << "Have a nice day." << endl;
    return 0;
}

void printIntro() {
	cout << "Welcome to CS 106B Word Ladder." << endl 
		 << "Please give my two English words, and I will change the" << endl
		 << "first into the second by changing one letter at a time." << endl;
}

string getFileString(string prompt) {
    while(true) {
        string fileString = getLine(prompt);
        if (fileExists(fileString)) return fileString;
        cout << "Unable to open that file. Try again." << endl;
    }
}

void formatWord(string &word) {
    for (char &c : word) {
        c = tolower(c);
	}
}

/* "checkWords" checks for the conditions necessary to be able to find
 * the word ladder of the two words and formats the strings if necessary. */
bool checkWords(string word1, string word2, Lexicon myLex) {
	if (word1.length() != word2.length()) {
        cout << "The two words must be the same length" << endl;
		return false;
	}

	if (word1 == word2) {
		cout << "The two words must be different" << endl;
		return false;
	}

    for (int i = 0; (size_t) i < word1.length(); i++) {
		if (!isalpha(word1[i]) || !isalpha(word2[i])) {
			cout << "The words must be alphanumeric" << endl;
			return false;
		}
	}

	if (!myLex.contains(word1) || !myLex.contains(word2)) {
		cout << "The two words must be found in the dictionary" << endl;
		return false;
	}
    return true;
}

Set<string> getNextWords(string word) {
    Set<string> nextWords;
    string alphabet = "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; (size_t) i < word.length(); i++) {
        for (char c : alphabet) {
            string next_word = word;
            next_word[i] = c;
            nextWords.add(next_word);
        }
    }
    return nextWords;
}

Stack<string> getLadder(string word1, string word2, Lexicon myLex) {
	Set<string> used_words;
    Stack<string> initial_ladder = {word1};
    Queue<Stack<string>> queue = {initial_ladder};

	while (!queue.isEmpty()) {
        Stack<string> current = queue.dequeue();
        string word = current.peek();

        Set<string> nextWords = getNextWords(word);
        for (string word : nextWords) {
            if (myLex.contains(word)) {
                if (!used_words.contains(word)) {
                    if (word == word2) {
                        current.push(word);
                        return current;
                    } else {
                        Stack<string> next_stack = current;
                        next_stack.push(word);
                        queue.enqueue(next_stack);
                    }
                }
            }
        }
	}
    return {};
}

void printResult(string word1, string word2, Stack<string> ladder) {
    cout << "A ladder from " << word2 << " back to " << word1 << ":" << endl;
    while (!ladder.isEmpty()) {
        string item = ladder.pop();
        cout << item << " ";
    }
    cout << endl;
}
