
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <map>
#include <stack>
#include <bitset>
#include <sstream>
using namespace std;
const int CHARNUM = 256;
const int MAX_ENTRIES = 513;
//makes a node to hold frequency, glyph, and l/r pointers for the huffman table or tree
struct huffNode {
	int frequency;
	unsigned char glyph;
	int left = -1;
	int right = -1;
};


void openFiles(ofstream&, const string&, int&);
void initFrequencyTable(map<unsigned char, int>&);
void fillFrequencyTable(map<unsigned char, int>&, ifstream&);
multimap<int, unsigned char> deleteEmptyGlyphs(map<unsigned char, int>&);
void createTable(multimap<int, unsigned char>&, huffNode*);

void main() {

	ofstream fileOut;
	string fileName;
	map<unsigned char, int> frequencyTable;
	huffNode huffTable[MAX_ENTRIES];
	multimap<int, unsigned char> sortedTable;
	cout << "Please enter a file name.\n";
	cin >> fileName;
	ifstream fileIn(fileName, ios::in | ios::binary);
	auto start = chrono::high_resolution_clock::now();
	initFrequencyTable(frequencyTable);
	fillFrequencyTable(frequencyTable, fileIn);
	sortedTable = deleteEmptyGlyphs(frequencyTable);
	createTable(sortedTable, huffTable);

	

	fileIn.close();
	fileOut.close();
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
}

/*
createTable:
creates a huffman table from the glyphTable

glyphTable format: glyphTable[frequency] = glyph


*/
void createTable(multimap<int, unsigned char> &glyphTable, huffNode* huffTable) {
	int m, h, f, n;
	huffNode temp;
	huffNode testTable[MAX_ENTRIES];
	int replacement;
	f = glyphTable.size();
	h = glyphTable.size() - 1;
	
	//Set the initial values for the huffman table (the glyphs from the frequency table)
	int j = 0;
	for (auto i : glyphTable) {
		testTable[j].frequency = i.first;
		testTable[j].glyph = i.second;
		j++;
	}
	//for every glyph in the table:
	for (int i = 0; i < (glyphTable.size() - 1); i++) {
		//mark(m) lower of relative slots 2 and 3 ([1] and [2])
		if (h > 1) {
			m = testTable[1].frequency <= testTable[2].frequency ? 1 : 2;
		}
		else { m = 1; }
		//move (m) to the next free slot
		testTable[f] = testTable[m];

		//move h to m
		testTable[m] = testTable[h];

		//check for reheap of m slot
		replacement = m;
		//if left is valid
		if ((m * 2) + 1 < h) {
			//if left < m slot, set left to replacement
			if (testTable[m].frequency > testTable[(m * 2) + 1].frequency) {
				replacement = (m * 2) + 1;

			}
			//if right is valid and less than replacement, set right to replacement
			if ((m * 2) + 2 < h &&  testTable[(m * 2) + 2].frequency < testTable[replacement].frequency) {
				replacement = (m * 2) + 2;
			}
		}
		//replace the m slot with replacement (if no replacement, m stays the same)
		temp = testTable[m];
		testTable[m] = testTable[replacement];
		testTable[replacement] = temp;
		//move slot 0 to (h)
		testTable[h] = testTable[0];

		//create freq node at 0
		testTable[0].frequency = testTable[h].frequency + testTable[f].frequency;
		testTable[0].glyph = -1;
		testTable[0].left = h;
		testTable[0].right = f;

		//check for reheap of all slots
		for (n = 0; n < h; n++) {

			replacement = n;
			//if left is valid
			if ((n * 2) + 1 < h) {
				//if left < n slot, set left to replacement
				if (testTable[n].frequency > testTable[(n * 2) + 1].frequency) {
					replacement = (n * 2) + 1;

				}
				//if right is valid and less than replacement, set right to replacement
				if ((n * 2) + 2 < h &&  testTable[(n * 2) + 2].frequency < testTable[replacement].frequency) {
					replacement = (n * 2) + 2;
				}
			}
			//replace the n slot with replacement (if no replacement, n stays the same)
			temp = testTable[n];
			testTable[n] = testTable[replacement];
			testTable[replacement] = temp;
		}

		h--;
		f++;

	}
	for (int i = 0; i < (glyphTable.size() * 2) - 1; i++) {
		cout << i << ") " << testTable[i].glyph << " : " << testTable[i].frequency;
		cout << ", " << testTable[i].left << " < --- > " << testTable[i].right << endl;
	}
}

//opens files, stupid
void openFiles(ofstream &fileOut, const string &fileName, int &fs) {
	auto start = chrono::high_resolution_clock::now();




	string outFileName = fileName.substr(0, fileName.length() - 4);
	outFileName = outFileName + ".huf";
	fileOut.open(outFileName, ios::out | ios::binary);
	if (!fileOut.is_open()) {
		cout << "fileOut failed to open.\n";
		fileOut.close();
		fs = 0;
	}

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
}

//populates map with glyphs and frequencies
void fillFrequencyTable(map<unsigned char, int>& frequencyTable, ifstream &fileIn) {
	auto start = chrono::high_resolution_clock::now();
	unsigned char glyph;

	while (fileIn.read((char*)&glyph, 1)) {
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