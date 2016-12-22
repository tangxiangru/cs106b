// Cristobal's Huffman Implementation
// Section Leader: Shirin Salehi

#include "encoding.h"
#include "pqueue.h"
#include "filelib.h"
#include "huffmanutil.h"

// Helper Function Definitions
void buildEncodingMap(HuffmanNode* curr, Map<int, string> &encodingMap, string &builder);

/* buildFrequencyTable:
 * --------------------------------------------------------------------
 * Reads the input a character at a time, adding it to the freqTable
 * map. If the input is -1, the file is finished. Then, add a EOF.
 */

Map<int, int> buildFrequencyTable(istream& input) {
    Map<int, int> freqTable;

    while(true) {
        int c = input.get();
        // c == -1 indicates EOF
        if (c == -1) break;
        // Increment if already in table
        if (freqTable.containsKey(c)) freqTable[c] ++;
        // Else, add to table
        else freqTable[c] = 1;
    }
    freqTable[PSEUDO_EOF] = 1;

    return freqTable;
}

/* buildEncodingTree:
 * --------------------------------------------------------------------
 * Create a PQueue and loop over freqTable adding HuffmanNode's with
 * the freqTable's values to the PQueue. Then, use the algorithm described
 * on the assignment's doc to succesively dequeue and enqueue to form
 * a tree with the correct structure.
 * Return the root to the tree.
 */

HuffmanNode* buildEncodingTree(const Map<int, int>& freqTable) {

    PriorityQueue<HuffmanNode*> pq;
    // Loop over freqTable, creating nodes and inserting them into PQueue
    for (int c : freqTable) {
        // Instantiate new HuffmanNode
        HuffmanNode *node = new HuffmanNode(c, freqTable[c], NULL, NULL);
        pq.enqueue(node, freqTable[c]);
    }

    // Create encoding tree through algorithm in instructions
    while (pq.size() > 1) {
        HuffmanNode *first = pq.dequeue();
        HuffmanNode *second = pq.dequeue();
        // Combine first and second nodes
        HuffmanNode *combo = new HuffmanNode(NOT_A_CHAR, first->count + second->count, first, second);
        pq.enqueue(combo, combo->count);
    }

    // The root of the encoding tree will be the last element on the PQueue
    return pq.dequeue();
}

/* buildEncodingMap (main function):
 * --------------------------------------------------------------------
 * Initialize the map variable and an auxiliary string that will be used
 * to recursively travel through the encoding tree and save the path taken.
 * Then call on the buildEncoding recursive helper to populate the map.
 * Return encodingMap.
 */

Map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    Map<int, string> encodingMap;
    string builder = "";
    buildEncodingMap(encodingTree, encodingMap, builder);
    return encodingMap;
}

/* buildEncodingMap (helper function):
 * --------------------------------------------------------------------
 * Base Case: we are at a leaf. In this case, the builder string contains
 * the encoding for the character we are currently at. Add this key, value
 * pair to the encoding map.
 * Recursive Case: add the step taken to the builder string and recurse in
 * both directions. ("0" if left, "1" if right).
 */

void buildEncodingMap(HuffmanNode* curr, Map<int, string> &encodingMap, string &builder) {
    // Base Case
    if (curr->isLeaf()) encodingMap[curr->character] = builder;
    else {
        // Recurse Left
        string zero = builder + "0";
        buildEncodingMap(curr->zero, encodingMap, zero);
        // Recurse Right
        string one = builder + "1";
        buildEncodingMap(curr->one, encodingMap, one);
    }
}

/* encodeData:
 * --------------------------------------------------------------------
 * Iterate over the input stream. On each iteration, retrieve the encoding
 * that corresponds to that character from the encoding map and output it
 * bit by bit using a for-loop. Then, loop over the encoding for EOF and
 * add it the output stream.
 */

void encodeData(istream& input, const Map<int, string>& encodingMap, obitstream& output) {
    while (true) {
        int c = input.get();
        // c == -1 indicates EOF
        if (c == -1) break;
        string code = encodingMap[c];
        // Loop over encoding to add bit by bit
        for (char i : code) {
            // Ternary to transform characters into bits
            output.writeBit(i == '1' ? 1 : 0);
        }
    }
    string eof_code = encodingMap[PSEUDO_EOF];
    for (char i : eof_code) {
        output.writeBit(i == '1' ? 1 : 0);
    }
}

/* decodeData:
 * --------------------------------------------------------------------
 * Make a copy of the root that will be used to travel along the tree.
 * If "curr" is a leaf, the character of curr is outputed and we return
 * to the root of the tree. Else, a direction is read from the input bitsream
 * and followed. Repeat until reach the EOF, where you break.
 */

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* curr = encodingTree;
    while (true) {
        if (curr->isLeaf()) {
            if (curr->character == PSEUDO_EOF) break;
            output.put(curr->character);
            // Reset curr to top of tree
            curr = encodingTree;
        }
        int b = input.readBit();
        if (input.fail()) break;
        // Go in the direction of b
        if (b == 0) curr = curr->zero;
        else curr = curr->one;
    }
}

/* compress:
 * --------------------------------------------------------------------
 * Uses the functions defined above in succession to create an encoding
 * map from the input. The freqTable is added to the output as a header.
 * Then input is then rewinded and encoded using the encodeData function,
 * which outputs the encoded message.
 */

void compress(istream& input, obitstream& output) {
    Map<int, int> freqTable = buildFrequencyTable(input);
    HuffmanNode* root = buildEncodingTree(freqTable);
    Map<int, string> encodingMap = buildEncodingMap(root);

    // In order to have an unmodified input to encode, create rewind
    rewindStream(input);
    output << freqTable;
    encodeData(input, encodingMap, output);
}

/* decompress:
 * --------------------------------------------------------------------
 * Extracts the freqTable from the input bitsream and uses it to generate
 * the encoding tree. Then, the rest of the input is decoded using the
 * decodeData function.
 */

void decompress(ibitstream& input, ostream& output) {
    // Extracting freqTable;
    Map<int, int> freqTable;
    input >> freqTable;
    // Create encoding tree and decode
    HuffmanNode* root = buildEncodingTree(freqTable);
    decodeData(input, root, output);
}

/* freeTree:
 * --------------------------------------------------------------------
 * Destruction of node once children are explored and destroyed.
 */

void freeTree(HuffmanNode* node) {
    // Post-order traversal of tree for destruction
    if (!node->isLeaf()) {
        freeTree(node->zero);
        freeTree(node->one);
    }
    delete node;
}
