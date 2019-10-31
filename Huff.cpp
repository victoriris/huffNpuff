
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <map>
#include <stack>
#include <bitset>
#include <vector>
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

//creates a bitset with the number of spaces used
struct hbitset {
	bitset<20> bits;
	int it;
};

void openFiles(ifstream&, ofstream&, const string&, int&);
void initGlyphTable(map<unsigned char, int>&);
void fillGlyphTable(map<unsigned char, int>&, ifstream&);
void writeHeader(ofstream&, string&, const multimap<int, unsigned char>&);
multimap<int, unsigned char> deleteEmptyGlyphs(map<unsigned char, int>&);
void buildTable(multimap<int, unsigned char>&, huffNode *&);
void writeTable(ofstream&, huffNode*&, const int&);
void createBitset(ofstream&, huffNode*&, map<char, hbitset>&);
void writeMessage(ifstream&, ofstream&, map<char, hbitset>&, const string&);

void main() {
	ifstream fileIn;
	ofstream fileOut;
	string fileName;
	map<char, hbitset> bitSets;
	map<unsigned char, int> glyphTable;
	int fs;

	cout << "Please enter a file name.\n";
	cin >> fileName;
	auto start = chrono::high_resolution_clock::now();

	openFiles(fileIn, fileOut, fileName, fs);
	if (fs) {
		initGlyphTable(glyphTable);
		fillGlyphTable(glyphTable, fileIn);
		multimap<int, unsigned char> sortedTable = deleteEmptyGlyphs(glyphTable);
		writeHeader(fileOut, fileName, sortedTable);
		huffNode* huffTable;
		int tableSize = (sortedTable.size() * 2) - 1;
		huffTable = new huffNode[tableSize];
		buildTable(sortedTable, huffTable);
		writeTable(fileOut, huffTable, tableSize);
		createBitset(fileOut, huffTable, bitSets);
		writeMessage(fileIn, fileOut, bitSets, fileName);
	}

	fileIn.close();
	fileOut.close();
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
	system("PAUSE");
}

//tries to write out the bitsets to the file
void writeMessage(ifstream& fileIn, ofstream& fileOut, map<char, hbitset>& bitSets, const string &fileName) {
	auto start = chrono::high_resolution_clock::now();
	unsigned char c = ' ';
	string bs;
	fileIn.close();
	fileIn.open(fileName, ios::in);
	while (!fileIn.eof()) {
		fileIn.read((char*)&c, sizeof c);
		bs = bitSets[c].bits.to_string().substr((20 - bitSets[c].it), bitSets[c].it);
		fileOut.write((char*)&bs.front(), bs.length());
	}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
}

//create a bitset for each char, in a map
void createBitset(ofstream& fileOut, huffNode*& huffTable, map<char, hbitset> &bitSets) {
	auto start = chrono::high_resolution_clock::now();
	stack<huffNode> S;
	int depth = 0;
	hbitset bitArray;
	istringstream bit_stream;
	huffNode root = huffTable[0];
	int blocker;
	string bitString = "";

	do {
		if (root.left > 0) {
			//block for stack so that it doesn't traverse the same way
			blocker = root.left;
			root.left = -1;
			//add to stack
			S.push(root);
			//move left
			root = huffTable[blocker];
			bitArray.bits << 1;
			bitArray.bits.set(0);
			depth++;
		}
		else if (root.left < 0 && root.right > 0)
		{
			blocker = root.right;
			root.right = -1;
			S.push(root);
			//move right
			root = huffTable[blocker];
			bitArray.bits << 1;
			bitArray.bits.set(1);
			depth++;
		}
		//if left and right are both -1, you've found a glyph. add the bitArray to the bitSet map.
		else if (!S.empty()) {
			bitArray.it = depth;
			bitSets[root.glyph] = bitArray;
			bitArray.bits >>= 1;
			depth--;
			root = S.top();
			S.pop();
		}
	} while (!S.empty());

	for (auto i : bitSets) {
		cout << i.first << " : ";
		cout << i.second.bits.to_string().substr((20 - i.second.it), i.second.it);
		cout << endl;
	}



	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
}
//writes the huffman table to the file
void writeTable(ofstream& fileOut, huffNode*& huffTable, const int& tableSize) {
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < tableSize; i++) {
		fileOut.write((char*)&huffTable[i].glyph, sizeof huffTable[i].glyph);
		fileOut.write((char*)&huffTable[i].left, sizeof huffTable[i].left);
		fileOut.write((char*)&huffTable[i].right, sizeof huffTable[i].right);
	}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
}
//builds the huffman table with the huffman algorithm (but also doesn't work I think)
void buildTable(multimap<int, unsigned char> &glyphTable, huffNode* &huffTable) {
	auto start = chrono::high_resolution_clock::now();
	huffNode tempNode;
	int m, f, h;
	int j = 0;
	f = glyphTable.size();
	h = glyphTable.size() - 1;
	for (auto i : glyphTable) {
		huffTable[j].frequency = i.first;
		huffTable[j].glyph = i.second;
		huffTable[j].left = -1;
		huffTable[j].right = -1;
		j++;
	}
	for (int k = 1; k < glyphTable.size(); k++) {
		if (huffTable[1].frequency >= huffTable[2].frequency) {
			m = 1;
		}
		else {
			m = 2;
		}
		huffTable[f] = huffTable[m];
		if (m < h) {
			huffTable[m] = huffTable[h];
		}


		for (int i = 1; i < h - 1; i++) {
			if (huffTable[i].frequency > huffTable[i + 1].frequency) {
				tempNode = huffTable[i];
				huffTable[i] = huffTable[i + 1];
				huffTable[i + 1] = tempNode;
			}
		}

		huffTable[h] = huffTable[0];
		huffTable[0].frequency = huffTable[h].frequency + huffTable[f].frequency;
		huffTable[0].glyph = -1;
		huffTable[0].left = h;
		huffTable[0].right = f;


		for (int i = 1; i < h - 1; i++) {
			if (huffTable[i].frequency > huffTable[i + 1].frequency) {
				tempNode = huffTable[i];
				huffTable[i] = huffTable[i + 1];
				huffTable[i + 1] = tempNode;
			}
		}

		h--;
		f++;

	}

	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
}
//writes header to the file
void writeHeader(ofstream& fileOut, string& fileName, const multimap<int, unsigned char>& glyphTable) {
	auto start = chrono::high_resolution_clock::now();
	int fileNameSize = fileName.length();

	int tableSize = glyphTable.size();
	fileOut.write((char*)&fileNameSize, 4);
	cout << fileNameSize;
	fileOut.write((char*)&fileName.front(), fileName.length());
	cout << fileName;
	fileOut.write((char*)&tableSize, sizeof tableSize);
	cout << tableSize << endl;
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
//populates map with glyphs and frequencies
void fillGlyphTable(map<unsigned char, int>& glyphTable, ifstream &fileIn) {
	auto start = chrono::high_resolution_clock::now();
	unsigned char glyph = ' ';
	while (!fileIn.eof()) {
		fileIn.read((char*)&glyph, sizeof glyph);
		glyphTable[glyph]++;

	}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
}
//creates empty glyph map with all available glyphs
void initGlyphTable(map<unsigned char, int> &glyphTable) {
	auto start = chrono::high_resolution_clock::now();
	for (int i = 0; i < CHARNUM; i++) {
		glyphTable[i] = 0;
	}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << endl;
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