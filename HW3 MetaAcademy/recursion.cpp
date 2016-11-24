/*
 * Cristobal Sciutto's Recursive methods for HW3: MetaAcademy 
 * Section Leader: Shirin Salehi
 *
 * This file defines the recursive methods that are executed
 * by meta.cpp. The methods are essentially a recursion tutorial
 * for the user.
 */

#include <math.h>
#include "recursion.h"
#include "map.h"
#include "vector.h"
#include "set.h"
#include "gwindow.h"
#include "gobjects.h"
#include "tokenscanner.h"
using namespace std;

/************************************
 *    Greatest Common Denominator   *
 ************************************/

/* GCD Function: recursively calculates the GCD of two numbers
 * using Euclid's algorithm.
 * Base Case: b is zero, and therefore the GCD is trivially found
 * Recursive Case: euclid's algorithm
 */

int gcd(int a, int b) {
	if (b == 0) {
		cout << "gcd(" << a << "," << b << ") = " << a << endl;
		return a;
	} else {
        cout << "gcd(" << a << "," << b << ") = gcd(" << b << "," << a % b << ")" << endl;
		return gcd(b, a % b);
	} 
}

/************************************
 *        Serpinskii Fractal        *
 ************************************/

/* Serpinskii Helper: does the actual recursion for the serpinskii demo.
 * If the order of the serpinskii triange is 1, draw the triangle. In any other
 * case, execute serpinskii() for order (n-1) for each of the 3 triangles that
 * composes that n-ordered triangle.
 * The recursion expands a tree of calls in which each leaf is a triangle of
 * of the final drawing.
 */

void serpinskiiHelper(GWindow &w, int leftX, int leftY, int size, int order) {
	if (order == 1) {
		w.drawLine(leftX, leftY, leftX + size, leftY);
		w.drawLine(leftX, leftY, leftX + size / 2, leftY + (size * sqrt(3)) / 2);
		w.drawLine(leftX + size, leftY, leftX + size / 2, leftY + (size * sqrt(3)) / 2);
	}
	else if (order > 1) {
		serpinskii(w, leftX, leftY, size / 2, order - 1);
		serpinskii(w, leftX + size / 2, leftY, size / 2, order - 1);
		serpinskii(w, leftX + size / 4, leftY + (size * sqrt(3)) / 4, size / 2, order - 1);
	}
}

/* Serpinskii Function: checks for inconsistent or base inputs that should
 * not be exectued. If all is OK, i.e. order is non-negative, calls the
 * serpinskiiHelper function which does the actual recursion. Note that if
 * order == 0, nothing is executed.
 */

void serpinskii(GWindow &w, int leftX, int leftY, int size, int order) {
    if (order < 0) {
        throw("Illegal level: Serpinski level must be non-negative");
    } else if (order != 0) {
        serpinskiiHelper(w, leftX, leftY, size, order);
    }
}

/************************************
 *            Flood Fill            *
 ************************************/


/* Flood Fill Helper: through exploration recursion, goes through all of
 * the adjacent pixels of a given color and changes them to the newColor
 * indicated. Returns the number of pixels changed.
 * Returns 1 + floodHelper(each adjacent pixel), and therefore recursively
 * maintains the sum of the pixels changed.

 * Uses a ternary operatior to check if the adjacent pixel is the same
 * color as the current pixel in order to execute that part of the recursion.
 * If that is not the case, adds 0 to the recursive addition and doesn't
 * call the floodHelper recursively.
 * */

int floodHelper(GBufferedImage& image, int x, int y, int newColor) {
    if (image.inBounds(x, y)) {
        int currentPixel = image.getRGB(x, y);
		image.setRGB(x, y, newColor);
		return 1 + ((image.getRGB(x + 1, y) == currentPixel) ? floodHelper(image, x + 1, y, newColor) : 0)
				 + ((image.getRGB(x - 1, y) == currentPixel) ? floodHelper(image, x - 1, y, newColor) : 0)
				 + ((image.getRGB(x, y + 1) == currentPixel) ? floodHelper(image, x, y + 1, newColor) : 0)
				 + ((image.getRGB(x, y - 1) == currentPixel) ? floodHelper(image, x, y - 1, newColor) : 0);
    }
    return 0;
}

/* Flood Fill: function called by meta.cpp. This function only checks
 * to make sure that the first pixel clicked is not already the correct
 * color, in which case the recursion is not exectuted and 0 is returned.
 * Else, the recursion is executed through a helper function.
 */

int floodFill(GBufferedImage& image, int x, int y, int newColor) {
	int pixelClicked = image.getRGB(x, y);
	if (pixelClicked != newColor) {
        return floodHelper(image, x, y, newColor);
	}
	return 0;
}

/************************************
 *     Personalized Curriculum      *
 ************************************/

/* Curriculum Helper: recursively goes through the prereqMap, printing
 * each new node it goes through and adding it to the printed set.
 * It is important to note that the recursive call comes before printing
 * out the node in order to print a study list in the correct order.
 * Base Case: course has no prereqs, i.e. is not a key in the
 * prereqMap.
 * Recursive Case: calls curriculumHelper on every prereq of the
 * given course.
 */

void curriculumHelper(Map<string, Vector<string>> & prereqMap, string goal, Set<string> & printed) {
	if (prereqMap.containsKey(goal)) {
		for (string item : prereqMap[goal]) {
			curriculumHelper(prereqMap, item, printed);
		}
	}
	if (!printed.contains(goal)) {
		cout << goal << endl;
		printed.add(goal);
	}
}

/* Personal Curriculum Function: creates a set which will store
 * the already printed items and then calls the recursive function
 * helper with that additional parameter called by reference.
 */

void personalCurriculum(Map<string, Vector<string>> & prereqMap, string goal) {
    Set<string> printed;
    curriculumHelper(prereqMap, goal, printed);
}

/************************************
 *         Generate Question        *
 ************************************/

/* getRandom: gets a random string from a vector of strings
 */

string getRandom(Vector<string> myVec) {
    int index = randomInteger(0, myVec.size() - 1);
	return myVec[index];
}

/* Generate Helper Function: recurses through the grammar tree,
 * randomly selecting the next rule. The function adds only terminals
 * into a sentence string which is modified by reference.
 * Since the tree iterates from left to right, and the rules are
 * ordered, the sentence is guaranteed to be in the right order.
 * Base Case: the symbol is a terminal and therefore is not a key
 * in the grammar and should be added to the sentence.
 * Recursive Case: call generateHelper on a randomly selected rule
 * from the grammar set of the current symbol. The rule is tokenized
 * and then iterated over with a while loop.
 */

void generateHelper(Map<string, Vector<string> > & grammar, string symbol, string & sentence) {
    if (!grammar.containsKey(symbol)) {
        sentence += symbol;
    } else {
        string nextOption = getRandom(grammar[symbol]);
        TokenScanner scanner(nextOption);
        while (scanner.hasMoreTokens()) {
            string nextSymbol = scanner.nextToken();
            generateHelper(grammar, nextSymbol, sentence);
        }
    }
}

/* Generate Function: creates a sentence string which is passed
 * by reference to the helper function. That function populates
 * the sentence recursively. Then, the sentence is returned.
 */

string generate(Map<string, Vector<string> > & grammar, string symbol) {
    string sentence;
    generateHelper(grammar, symbol, sentence);
    return sentence;
}
