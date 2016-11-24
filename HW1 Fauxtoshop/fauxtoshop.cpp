// This is my first CS 106B assignment \o/
// FakeVim mode FTW!

#include <iostream>
#include "console.h"
#include "gwindow.h"
#include "grid.h"
#include "simpio.h"
#include "strlib.h"
#include "gbufferedimage.h"
#include "gevents.h"
#include "math.h" //for sqrt and exp in the optional Gaussian kernel
using namespace std;

static const int    WHITE = 0xFFFFFF;
static const int    BLACK = 0x000000;
static const int    GREEN = 0x00FF00;
static const double PI    = 3.14159265;

void    doFauxtoshop(GWindow &gw, GBufferedImage &img);

// MY HELPER FUNCTION DEFINITIONS
bool addImageToCanvas(string fileString, GWindow &gw, GBufferedImage &img);
int getChoice();
void scatterImage(GBufferedImage &img, int degree);
int deltaRGB(int &pixel1, int &pixel2);
int deltaRGBEuclidean(int &pixel1, int &pixel2);
bool isEdge(Grid<int> &grid, int i, int j, int threshold, int type_delta);
void findEdges(GBufferedImage &img, int threshold);
void addSticker(GBufferedImage &img, GBufferedImage &sticker, int tolerance);

bool	openImageFromFilename(GBufferedImage& img, string filename);
bool 	saveImageToFilename(const GBufferedImage &img, string filename);
void	getMouseClickLocation(int &row, int &col);

/* STARTER CODE FUNCTION - DO NOT EDIT
 *
 * This main simply declares a GWindow and a GBufferedImage for use
 * throughout the program. By asking you not to edit this function,
 * we are enforcing that the GWindow have a lifespan that spans the
 * entire duration of execution (trying to have more than one GWindow,
 * and/or GWindow(s) that go in and out of scope, can cause program
 * crashes).
 */
int main() {
    GWindow gw;
    gw.setTitle("Fauxtoshop");
    gw.setVisible(true);
    GBufferedImage img;
    doFauxtoshop(gw, img);
    return 0;
}

/* This is yours to edit. Depending on how you approach your problem
 * decomposition, you will want to rewrite some of these lines, move
 * them inside loops, or move them inside helper functions, etc.
 *
 * The following is my code :)
 */
void doFauxtoshop(GWindow &gw, GBufferedImage &img) {

    while (true) {
        cout << "Welcome to Fauxtoshop!" << endl;
        string fileString;
        bool condition = false;
        while (condition == false) {
            fileString = getLine("Enter name of image file to open (or blank to quit): ");
            condition = fileString.empty() || addImageToCanvas(fileString, gw, img);
        }

        if (fileString.empty()) {
            break;
        }

        int choice = getChoice();

        if (choice == 1) {
            int degree;
            do {
                degree = getInteger("Enter degree of scatter [1-100]: ");
            } while (degree > 100 || degree < 1);

            scatterImage(img, degree);
        }

        else if (choice == 2) {
			int threshold = getInteger("Enter threshold for edge detection: ");
            findEdges(img, threshold);
        }

        else if (choice == 3) {
            cout << "Now choose another file to add to your background image." << endl;
			string stickerString = getLine("Enter name of image file to open: ");
            GBufferedImage sticker;
            cout << "Opening image file, may take a minute..." << endl;
            openImageFromFilename(sticker, stickerString);
            int tolerance = getInteger("Now choose a tolerance threshold: ");

			addSticker(img, sticker, tolerance);
        }

        else if (choice == 4) {
            cout << "Now choose another image file to compare to." << endl;
            string compareString = getLine("Enter name of image file to open: ");
            GBufferedImage compareImg;
            cout << "Opening image file, may take a minute..." << endl;
            openImageFromFilename(compareImg, compareString);
            int diffPixels = img.countDiffPixels(compareImg);
            cout << "These images differ in " << diffPixels << " pixel locations!" << endl;
        }

        string outputFile;
        do {
            outputFile = getLine("Enter filename to save image (or blank to skip saving): ");
            if (outputFile.empty()) {
                break;
            }
            condition = saveImageToFilename(img, outputFile);
        } while (!condition);

        cout << endl;
    }
    gw.clear();
}

// MY HELPER FUNCTIONS
bool addImageToCanvas(string fileString, GWindow &gw, GBufferedImage &img) {
    // Adds borderplate to the openImageFromFilename function and adds the image to the canvas
    if (openImageFromFilename(img, fileString)) {
		cout << "Opening image file, may take a minute..." << endl;
		gw.setCanvasSize(img.getWidth(), img.getHeight());
		gw.add(&img, 0, 0);
        return true;
    } else { return false; };
}

// Prints the interface and returns the user's choice
int getChoice() {
	cout << "Which image filter would you like to apply?" << endl
		 << "\t1 - Scatter" << endl
		 << "\t2 - Edge detection" << endl
		 << "\t3 - \"Green screen\" with another image" << endl
         << "\t4 - Compare image with another image" << endl;
	int choice = getInteger("Your choice: ");
	return choice;
}

// Creates the corresponding grids and scatters the image, adding it to the canvas
void scatterImage(GBufferedImage &img, int degree) {
    Grid<int> imgGrid = img.toGrid();
    int cols = imgGrid.numCols(), rows = imgGrid.numRows();
    Grid<int> resultGrid = Grid<int>(rows, cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            int new_i, new_j;
            int radius = randomInteger(0, degree);
            do {
                new_i = randomInteger(i - radius, i + radius);
                new_j = randomInteger(j - radius, j + radius);
            } while (!imgGrid.inBounds(new_i, new_j));
            
			int pixel = imgGrid[new_i][new_j];
            resultGrid.set(i, j, pixel);
        }
    }
    img.fromGrid(resultGrid);
}

// Compares two grid pixelss and returns the value of their difference
int deltaRGB(int &pixel1, int &pixel2) {
	int red1, green1, blue1;
	int red2, green2, blue2;
	int delta_red, delta_green, delta_blue;
	GBufferedImage::getRedGreenBlue(pixel1, red1, green1, blue1);
	GBufferedImage::getRedGreenBlue(pixel2, red2, green2, blue2);
	
    delta_red = abs(red2 - red1);
    delta_green = abs(green2 - green1);
    delta_blue = abs(blue2 - blue1);

    return max(delta_red, max(delta_green, delta_blue));
}

// Compares two grid pixelss and returns the value of their difference
int deltaRGBEuclidean(int &pixel1, int &pixel2) {
    int red1, green1, blue1;
    int red2, green2, blue2;
    int delta_red, delta_green, delta_blue;
    GBufferedImage::getRedGreenBlue(pixel1, red1, green1, blue1);
    GBufferedImage::getRedGreenBlue(pixel2, red2, green2, blue2);

    delta_red = abs(red2 - red1);
    delta_green = abs(green2 - green1);
    delta_blue = abs(blue2 - blue1);

    return sqrt(pow(delta_red, 2) + pow(delta_green,2) + pow(delta_blue,2));
}

// Applies the deltaRGB function to all of the adjacent pixels, checking if one of them surpasses the threshold
bool isEdge(Grid<int> &grid, int i, int j, int threshold, int type_delta) {
	int pixel = grid[i][j];
	int compared_pixel;
    int difference;
    for (int m = i - 1; m < i + 2; m++) {
        for (int n = j - 1; n < j + 2; n++) {
            if (grid.inBounds(m, n)) {
                compared_pixel = grid[m][n];
                if (type_delta == 1) {
                    difference = deltaRGB(pixel, compared_pixel);
                } else if (type_delta == 2) {
                    difference = deltaRGBEuclidean(pixel, compared_pixel);
                }
                if (difference > threshold) {
                    return true;
                }
            }
        }
    }
	return false;
}

// Using the isEdge function, edits the image to BLACK and WHITE, depending if the pixel is an edge
void findEdges(GBufferedImage &img, int threshold) {
    Grid<int> imgGrid = img.toGrid();
    int cols = imgGrid.numCols();
    int rows = imgGrid.numRows();
    Grid<int> resultGrid = Grid<int>(rows, cols);

    int type_delta = getInteger("Would you like to use the normal RGB difference or the Euclidean difference [1 or 2]?");

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (isEdge(imgGrid, i, j, threshold, type_delta)) {
				resultGrid.set(i, j, BLACK);	
			} else {
				resultGrid.set(i, j, WHITE);
            }
        }
    }
    img.fromGrid(resultGrid);
} 

// Takes a sticker image, and adds it pixel-by-pixel to the background image if the deltaRGB
// is greater than a tolerance threshold
void addSticker(GBufferedImage &img, GBufferedImage &sticker, int tolerance) {
    Grid<int> imgGrid = img.toGrid();
	Grid<int> stickerGrid = sticker.toGrid();
    int cols = stickerGrid.numCols();
    int rows = stickerGrid.numRows();

    int row, col;
	string coords = getLine("Enter location to place image as \"(row,col)\" (or blank to use mouse)");
	int index = coords.find(',');
    if (!coords.empty()) {
        row = stoi(coords.substr(1, index));
        col = stoi(coords.substr(index + 1, coords.length()));
    } else {
        getMouseClickLocation(row, col);
    }
	
    int greenPixel = GREEN;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
			int pixel = stickerGrid[i][j];
            if (deltaRGB(pixel, greenPixel) > tolerance) {
                if (imgGrid.inBounds(row + i, col + j)) {
                    imgGrid.set(row + i, col + j, pixel);
                }
            }
        }
    }
    img.fromGrid(imgGrid);
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to open the image file 'filename'.
 *
 * This function returns true when the image file was successfully
 * opened and the 'img' object now contains that image, otherwise it
 * returns false.
 */
bool openImageFromFilename(GBufferedImage& img, string filename) {
    try { img.load(filename); }
    catch (...) { return false; }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Attempts to save the image file to 'filename'.
 *
 * This function returns true when the image was successfully saved
 * to the file specified, otherwise it returns false.
 */
bool saveImageToFilename(const GBufferedImage &img, string filename) {
    try { img.save(filename); }
    catch (...) { return false; }
    return true;
}

/* STARTER CODE HELPER FUNCTION - DO NOT EDIT
 *
 * Waits for a mouse click in the GWindow and reports click location.
 *
 * When this function returns, row and col are set to the row and
 * column where a mouse click was detected.
 */
void getMouseClickLocation(int &row, int &col) {
    GMouseEvent me;
    do {
        me = getNextEvent(MOUSE_EVENT);
    } while (me.getEventType() != MOUSE_CLICKED);
    row = me.getY();
    col = me.getX();
}

/* OPTIONAL HELPER FUNCTION
 *
 * This is only here in in case you decide to impelment a Gaussian
 * blur as an OPTIONAL extension (see the suggested extensions part
 * of the spec handout).
 *
 * Takes a radius and computes a 1-dimensional Gaussian blur kernel
 * with that radius. The 1-dimensional kernel can be applied to a
 * 2-dimensional image in two separate passes: first pass goes over
 * each row and does the horizontal convolutions, second pass goes
 * over each column and does the vertical convolutions. This is more
 * efficient than creating a 2-dimensional kernel and applying it in
 * one convolution pass.
 *
 * This code is based on the C# code posted by Stack Overflow user
 * "Cecil has a name" at this link:
 * http://stackoverflow.com/questions/1696113/how-do-i-gaussian-blur-an-image-without-using-any-in-built-gaussian-functions
 *
 */
Vector<double> gaussKernelForRadius(int radius) {
    if (radius < 1) {
        Vector<double> empty;
        return empty;
    }
    Vector<double> kernel(radius * 2 + 1);
    double magic1 = 1.0 / (2.0 * radius * radius);
    double magic2 = 1.0 / (sqrt(2.0 * PI) * radius);
    int r = -radius;
    double div = 0.0;
    for (int i = 0; i < kernel.size(); i++) {
        double x = r * r;
        kernel[i] = magic2 * exp(-x * magic1);
        r++;
        div += kernel[i];
    }
    for (int i = 0; i < kernel.size(); i++) {
        kernel[i] /= div;
    }
    return kernel;
}
