#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <map>
#include <vector>
#include <string>
using namespace std;


// Entry of huffman table
struct HuffTableItem {
    int glyph = -1;
    int left = -1;
    int right = -1;
};

// Huff File content and its decoding data, including th final byte codes
struct HuffFile {
    string name = "";
	ifstream compressedFile;
    string fileName;
    HuffTableItem hufTable[513];
    map<string, int> byteCodes;
};

// Determines if bit is not zero
bool isBitOn(unsigned char byte, int bitPosition) {
    return !!(byte & (short) pow(2, bitPosition));
}

// Get the file name and it's length
void getFileName(HuffFile &huffFile) {
    auto bitCount = 0, nameLength = 0;
	unsigned char c;

	// Count length of file name
    for (auto i = 0; i < 4; i++) {
        huffFile.compressedFile.read((char *)&c, 1);
        huffFile.compressedFile.seekg(0, 1);
        for (int j = 0; j < 8; j++) {
            nameLength += isBitOn(c, j) ? (short) pow(2, bitCount) : 0;
            bitCount++;
        }
    }

	// Get the name of file
    for (int i = 0; i < nameLength; i++) {
        huffFile.compressedFile.read((char *)&c, 1);
        huffFile.compressedFile.seekg(0, 1);
        huffFile.name += c;
    }
}

// Get huffman table
// Uses file's content, gets the length and gets the values for the table
void getHuffTable(HuffFile &huffFile) {
    auto bitCount = 0, bitValue = 0, glyphCount = 0;
	unsigned char c;

	// Get length of huffman table
    for (auto i = 0; i < 4; i++) {
        huffFile.compressedFile.read((char *)&c, 1);
        huffFile.compressedFile.seekg(0, 1);
        for (auto j = 0; j < 8; j++) {
            glyphCount += isBitOn(c, j) ? (short) pow(2, bitCount) : 0;
            bitCount++;
        }
    }

    // For every slot in huffman table
	for (auto slot = 0; slot < glyphCount; slot++) {

		// For every field (glyph, left & right)
        for (auto field = 0; field < 3; field++) {
            bitCount = 0;
            bitValue = 0;

			// For every value from the file (4 bytes)
            for (int k = 0; k < 4; k++) {
                huffFile.compressedFile.read((char *)&c, 1);
                huffFile.compressedFile.seekg(0, 1);
				// For every bit
                for (int l = 0; l < 8; l++) {
                    bitValue += isBitOn(c, l) ? (short) pow(2.0, bitCount) : 0;
                    bitCount++;
                }
            }

			// Assign bit value to appropiate table field
            if (field == 0) huffFile.hufTable[slot].glyph = bitValue;
            else if (field == 1) huffFile.hufTable[slot].left = bitValue;
            else huffFile.hufTable[slot].right = bitValue;
        }
    }

}

// Generate byte codes
void getByteCodes(HuffTableItem huffTable[], map<string, int> &byteCodes, int position, string byteCode = "") {
    
	// If is a leaf, attach the bytecode to glyph
	if (huffTable[position].left == -1 && huffTable[position].right == -1) {
		byteCodes[byteCode] = huffTable[position].glyph;
		return;
	}

	// Otherwise.. is a merge

	// If left, add zero to byte code
	if (huffTable[position].left != -1) {
		getByteCodes(huffTable, byteCodes, huffTable[position].left, (byteCode + "0"));
	}
	// If right, add 1
	if (huffTable[position].right != -1) {
		getByteCodes(huffTable, byteCodes, huffTable[position].right, (byteCode + "1"));
	}
	
}

// Get file content to decode
string getContent(HuffFile &huffFile) {
	string fileContent = "";

    // Current position in the file
    auto position = huffFile.compressedFile.tellg();

	// Get file size
    huffFile.compressedFile.seekg(0, huffFile.compressedFile.end);
    auto fileSize = huffFile.compressedFile.tellg();
    huffFile.compressedFile.seekg(position);
    
	// For every byte 
    for (int i = position; i < fileSize; i++) {
        unsigned char c;
        huffFile.compressedFile.read((char *)&c, 1);
        huffFile.compressedFile.seekg(0, 1);
		// Determine its status and add bit to string
        for (int j = 0; j < 8; j++) {
            fileContent += isBitOn(c, j) ? "1" : "0";
        }
    }
    return fileContent;
}

// Get decoded content
string decodeContent(HuffFile &huffFile) {
	string comparisonString;
	string decodedContent;
	string codedContent = getContent(huffFile);
    auto length = codedContent.length();

	// For every bit
    for (auto i = 0; i < length; i++) {
		// Read and compare it with byte codes and glyphs map
        comparisonString += codedContent[i];
        auto search = huffFile.byteCodes.find(comparisonString);
        if (search != huffFile.byteCodes.end()) {
			// If EOF, stop and return the message
            if ( search->second == 0) {
                i = length;
            } 
			// If matches a byte code, append glyph
			else {
                decodedContent += (unsigned char) search->second;
				// Reset the comparison string
                comparisonString = "";
            }
        }
    }
    return decodedContent;
}


int main() {
	
	// Input of file
    string inFileName;
    cout << "Compressed Filename: ";
    cin >> inFileName;

	// Start timer
	auto start = chrono::high_resolution_clock::now();

	// Initialize huff file and load its content
	HuffFile huffFile;
	huffFile.fileName = inFileName;
	huffFile.compressedFile = ifstream(huffFile.fileName, ios::in | ios::binary);

	// Process file
	getFileName(huffFile);
	getHuffTable(huffFile);
	getByteCodes(huffFile.hufTable, huffFile.byteCodes, 0);

	// Decode content and write file
	auto decodedContent = decodeContent(huffFile);
	ofstream decodedFile(huffFile.name, ios::out | ios::binary);
	decodedFile.write(decodedContent.c_str(), decodedContent.size());
	decodedFile.close();


	// Stop and print timer
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << "Duration: " << duration.count();
	cout << " microseconds" << endl;
	
	decodedFile.close();
	return 0;

}