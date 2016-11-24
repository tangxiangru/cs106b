// Cristobal Sciutto's Ngram solution

#include <cctype>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include "console.h"
#include "simpio.h"
#include "queue.h"
#include "map.h"
#include "strlib.h"

using namespace std;

// Function Declarations
void printIntro();
ifstream getFile();
string getRandomString(Vector<string> vect);
void printVector(Vector<string> vect);
Map<Vector<string>,Vector<string>> createMap(ifstream &infile, int n);
void generateText(int num, Map<Vector<string>, Vector<string>> map);

int main() {
    printIntro();  
    ifstream infile = getFile();

    int n = getInteger("Value of N?");
    while (n < 2) {
        cout << "N must be 2 or greater." << endl;
        n = getInteger("Value of N?");
    }

    Map<Vector<string>, Vector<string>> myMap = createMap(infile, n);
    cout << endl;

    while (true) {
        int num = getInteger("# of random words to generate (0 to quit)?");
        if (num == 0) break;

        while (num < n) {
            cout << "Must be at least " << n << " words." << endl;
            num = getInteger("# of random words to generate (0 to quit)?");
        }

        generateText(num, myMap);
        cout << endl;
    }

    cout << "Exiting." << endl;
    return 0;
}

/* Prints out the basic introduction of the program. */

void printIntro() {
    cout << "Welcome to CS 106B Random Writer ('N-Grams')." << endl
         << "This program makes random text based on a document." << endl
         << "Give me an input file and an 'N' value for groups" << endl
         << "of words, and I'll create random text for you."<< endl << endl;
}

/* Takes the user's input for a filename, making sure it is valid,
 * and returns the corresponding ifstream of that path. */

ifstream getFile() {
    ifstream infile;
    while (true) {
        string filename = getLine("Input file name? ");
        infile.open(filename);
        if (!infile.fail()) return infile;
        cout << "Unable to open that file. Try again." << endl;
    }
}

/* Given a vector of strings, returns a random item of the vector. */

string getRandomString(Vector<string> vect) {
    int randInt = randomInteger(0, vect.size() - 1);
    return vect[randInt];
}

/* Given a vector of strings, prints out item by item on a line with spaces between them. */

void printVector(Vector<string> vect) {
    for (string item : vect) {
        cout << item << " ";
    }
}

/* Given a file ifstream, and the number N, creates a map of (N-1)-grams to
 * a vector with all of the possible Nth elements. This is done with a constantly
 * updating window vector which "slides" through the file. The word immediately
 * after the window is added to the map, and then the window is updated. To be
 * able to loop back to the beginning of the file, the first (n-1) elements are
 * held in a variable first_window, which is then looped through for the final
 * combinations. */

Map<Vector<string>,Vector<string>> createMap(ifstream &infile, int n) {
    Map<Vector<string>,Vector<string>> map;
    Vector<string> window;
    Vector<string> first_window;

    for (int i = 1; i < n; i++) {
		string next_word;
		infile >> next_word;
        first_window.add(next_word);
	}
	window = first_window;
	
	while (true) {
        string next_word;
		infile >> next_word;
        if (infile.fail()) break;
        if (map.containsKey(window)) {
            map[window] += next_word;
        } else {
            map[window] = {next_word};
        }
        window.remove(0);
        window.add(next_word);
	}

	for (string str : first_window) {
		map[window] += str;
        window.remove(0);
        window.add(str);
	}

    infile.close();
    return map;
}

/* Given the map generated in the function createMap(), random sequences of N words
 * are concatenated together to form a string of "num" words. The randomness is weighted
 * according to the number of appearances of such N-gram in the overall text from which
 * the map was extracted. */

void generateText(int num, Map<Vector<string>, Vector<string>> map) {
    Vector<Vector<string>> keys = map.keys();
    int randInt = randomInteger(0, keys.size() - 1);
    Vector<string> window_key = keys[randInt];
    int n = window_key.size();

    Vector<string> myStory;
    myStory += window_key;

    for (int i = 0; i < num - n; i++) {
        string next_word = getRandomString(map[window_key]);
        myStory.add(next_word);
        window_key.remove(0);
        window_key.add(next_word);
    }

    cout << "... ";
    printVector(myStory);
    cout << "..." << endl;
}
