
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <map>
#include <stack>
#include <bitset>
#include <sstream>
using namespace std;
const int BITNUM = 20;
const int CHARNUM = 256;
//makes a node to hold frequency, glyph, and l/r pointers for the huffman table or tree
struct huffNode {
	int frequency;
	unsigned char glyph;
	int left;
	int right;
};


void openFiles(ifstream&, ofstream&, const string&, int&);
void initFrequencyTable(map<unsigned char, int>&);
void fillFrequencyTable(map<unsigned char, int>&, ifstream&);
multimap<int, unsigned char> deleteEmptyGlyphs(map<unsigned char, int>&);

void main() {
	ifstream fileIn;
	ofstream fileOut;
	string fileName;
	map<unsigned char, int> frequencyTable;
	multimap<int, unsigned char> sortedTable;
	cout << "Please enter a file name.\n";
	cin >> fileName;
	auto start = chrono::high_resolution_clock::now();
	initFrequencyTable(frequencyTable);
	fillFrequencyTable(frequencyTable, fileIn);
	sortedTable = deleteEmptyGlyphs(frequencyTable);


	fileIn.close();
	fileOut.close();
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
	system("PAUSE");
}

//opens files, stupid
void openFiles(ifstream &fileIn, ofstream &fileOut, const string &fileName, int &fs) {
	auto start = chrono::high_resolution_clock::now();
	fileIn.open(fileName, ios::in | ios::binary);
	if (!fileIn.is_open()) {
		cout << "fileIn failed to open.\n";
		fileIn.close();
		fs = 0;
	}
	else {
		fs = 1;
	}

	if (fs == 1) {
		string outFileName = fileName.substr(0, fileName.length() - 4);
		outFileName = outFileName + ".huf";
		fileOut.open(outFileName, ios::out | ios::binary);
		if (!fileOut.is_open()) {
			cout << "fileOut failed to open.\n";
			fileOut.close();
			fs = 0;
		}
	}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
}

//populates map with glyphs and frequencies
void fillFrequencyTable(map<unsigned char, int>& frequencyTable, ifstream &fileIn) {
	auto start = chrono::high_resolution_clock::now();
	unsigned char glyph;
	
	while (!fileIn.eof()) {
		fileIn.read((char*)&glyph, 1);
		frequencyTable[glyph]++;

	}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
}
//creates empty glyph map with all available glyphs
void initFrequencyTable(map<unsigned char, int> &frequencyTable) {
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < CHARNUM; i++) {
		frequencyTable[i] = 0;
	}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
}

//deletes empty glyphs from the map, leaving only glyphs with positive frequency
multimap<int, unsigned char> deleteEmptyGlyphs(map<unsigned char, int>& glyphTable) {
	auto start = chrono::high_resolution_clock::now();

	for (int i = 0; i < CHARNUM; i++) {
		if (glyphTable[i] == 0) {
			glyphTable.erase(i);
		}
	}

	multimap<int, unsigned char> result;

	for (auto i : glyphTable) {
		result.insert({ i.second, i.first });
	}

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;

	return result;
}