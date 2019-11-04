////Connor Albers and Victor Iris
//#include <iostream>
//#include <fstream>
//#include <string>
//#include <chrono>
//#include <map>
//#include <stack>
//#include <bitset>
//#include <vector>
//#include <sstream>
//using namespace std;
//const int CHARNUM = 256;
//const int MAX_ENTRIES = 513;
////makes a node to hold frequency, glyph, and l/r pointers for the huffman table or tree
//struct huffNode {
//	int frequency;
//	int glyph = -1;
//	int left = -1;
//	int right = -1;
//};
//
//
//void openFiles(ifstream&, ofstream&, const string&, const int&);
//void initFrequencyTable(map<unsigned char, int>&);
//void fillFrequencyTable(map<unsigned char, int>&, ifstream&, int&);
//multimap<int, unsigned char> deleteEmptyGlyphs(map<unsigned char, int>&);
//void createTable(multimap<int, unsigned char>&, huffNode*);
//void createBitset(ofstream&, huffNode[], map<unsigned char, string>&);
//void writeMessage(ifstream&, ofstream&, map<unsigned char, string>&, const string&, const int &);
//void writeHeader(ofstream&, string&, const multimap<int, unsigned char>&);
//void writeTable(ofstream&, huffNode[], const int&);
//
//void main() {
//	ofstream fileOut;
//	ifstream fileIn;
//	string fileName;
//	map<unsigned char, int> frequencyTable;
//	huffNode huffTable[MAX_ENTRIES];
//	multimap<int, unsigned char> sortedTable;
//	map<unsigned char, string> bitSets;
//	cout << "Please enter a file name.\n";
//	cin >> fileName;
//	int fs;
//	openFiles(fileIn, fileOut, fileName, fs);
//	auto start = chrono::high_resolution_clock::now();
//	int fileSize = 1;
//	initFrequencyTable(frequencyTable);
//	fillFrequencyTable(frequencyTable, fileIn, fileSize);
//	sortedTable = deleteEmptyGlyphs(frequencyTable);
//	createTable(sortedTable, huffTable);
//	createBitset(fileOut, huffTable, bitSets);
//	writeHeader(fileOut, fileName, sortedTable);
//	int tableSize = (sortedTable.size() * 2) + 1;
//	writeTable(fileOut, huffTable, tableSize);
//	writeMessage(fileIn, fileOut, bitSets, fileName, fileSize);
//
//	fileIn.close();
//	fileOut.close();
//	auto stop = chrono::high_resolution_clock::now();
//	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
//	cout << duration.count() << endl;
//	system("pause");
//}
//
///*
//createTable:
//creates a huffman table from the glyphTable
//
//glyphTable format: glyphTable[frequency] = glyph
//
//
//*/
//void createTable(multimap<int, unsigned char> &glyphTable, huffNode* huffTable) {
//	int m, h, f, n;
//	huffNode temp;
//	int replacement;
//	f = glyphTable.size();
//	h = glyphTable.size() - 1;
//	
//	//Set the initial values for the huffman table (the glyphs from the frequency table)
//	int j = 0;
//	for (auto i : glyphTable) {
//		huffTable[j].frequency = i.first;
//		huffTable[j].glyph = i.second;
//		j++;
//	}
//	//for every glyph in the table:
//	for (int i = 0; i < (glyphTable.size() - 1); i++) {
//		//mark(m) lower of relative slots 2 and 3 ([1] and [2]) while not the last one
//		if (h > 1) {
//			m = huffTable[1].frequency <= huffTable[2].frequency ? 1 : 2;
//		}
//		else { m = 1; }
//		//move (m) to the next free slot
//		huffTable[f] = huffTable[m];
//
//		//move h to m
//		huffTable[m] = huffTable[h];
//
//		//check for reheap of m slot
//		replacement = m;
//		//if left is valid
//		if ((m * 2) + 1 < h) {
//			//if left < m slot, set left to replacement
//			if (huffTable[m].frequency > huffTable[(m * 2) + 1].frequency) {
//				replacement = (m * 2) + 1;
//
//			}
//			//if right is valid and less than replacement, set right to replacement
//			if ((m * 2) + 2 < h &&  huffTable[(m * 2) + 2].frequency < huffTable[replacement].frequency) {
//				replacement = (m * 2) + 2;
//			}
//		}
//		//replace the m slot with replacement (if no replacement, m stays the same)
//		temp = huffTable[m];
//		huffTable[m] = huffTable[replacement];
//		huffTable[replacement] = temp;
//		//move slot 0 to (h)
//		huffTable[h] = huffTable[0];
//
//		//create freq node at 0
//		huffTable[0].frequency = huffTable[h].frequency + huffTable[f].frequency;
//		huffTable[0].glyph = -1;
//		huffTable[0].left = h;
//		huffTable[0].right = f;
//
//		//check for reheap of all slots
//		for (n = 0; n < h; n++) {
//
//			replacement = n;
//			//if left is valid
//			if ((n * 2) + 1 < h) {
//				//if left < n slot, set left to replacement
//				if (huffTable[n].frequency > huffTable[(n * 2) + 1].frequency) {
//					replacement = (n * 2) + 1;
//
//				}
//				//if right is valid and less than replacement, set right to replacement
//				if ((n * 2) + 2 < h &&  huffTable[(n * 2) + 2].frequency < huffTable[replacement].frequency) {
//					replacement = (n * 2) + 2;
//				}
//			}
//			//replace the n slot with replacement (if no replacement, n stays the same)
//			temp = huffTable[n];
//			huffTable[n] = huffTable[replacement];
//			huffTable[replacement] = temp;
//		}
//
//		h--;
//		f++;
//
//	}
//	//for (int i = 0; i < (glyphTable.size() * 2) - 1; i++) {
//	//	cout << i << ") " << huffTable[i].glyph << " : " << huffTable[i].frequency;
//	//	cout << ", " << huffTable[i].left << " < --- > " << huffTable[i].right << endl;
//	//}
//}
//
//
////populates map with glyphs and frequencies
//void fillFrequencyTable(map<unsigned char, int>& frequencyTable, ifstream &fileIn, int& fileSize) {/*
//	auto start = chrono::high_resolution_clock::now();*/
//	unsigned char glyph;
//
//	while (fileIn.read((char*)&glyph, 1)) {
//		frequencyTable[glyph]++;
//		fileSize++;
//	}/*
//	auto stop = chrono::high_resolution_clock::now();
//	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
//	cout << duration.count() << endl;*/
//}
////creates empty glyph map with all available glyphs
//void initFrequencyTable(map<unsigned char, int> &frequencyTable) {/*
//	auto start = chrono::high_resolution_clock::now();*/
//	for (int i = 0; i < CHARNUM; i++) {
//		frequencyTable[i] = 0;
//	}
//	frequencyTable[256] = 1;/*
//	auto stop = chrono::high_resolution_clock::now();
//	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
//	cout << duration.count() << endl;*/
//}
//
////deletes empty glyphs from the map, leaving only glyphs with positive frequency
//multimap<int, unsigned char> deleteEmptyGlyphs(map<unsigned char, int>& glyphTable) {/*
//	auto start = chrono::high_resolution_clock::now();*/
//
//	for (int i = 0; i < CHARNUM; i++) {
//		if (glyphTable[i] == 0) {
//			glyphTable.erase(i);
//		}
//	}
//
//	multimap<int, unsigned char> result;
//
//	for (auto i : glyphTable) {
//		result.insert({ i.second, i.first });
//	}/*
//
//	auto stop = chrono::high_resolution_clock::now();
//	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
//	cout << duration.count() << endl;*/
//
//	return result;
//}
//
////create a bitset for each char, in a map
//void createBitset(ofstream& fileOut, huffNode huffTable[], map<unsigned char, string> &bitSets) {
//	//auto start = chrono::high_resolution_clock::now();
//	stack<huffNode> S;
//	int stringLength;
//	int cnt;
//	huffNode root;
//	int blocker;
//	string bitString = "";
//	root.left = root.right = -2;
//	root.frequency = 0;
//	root.glyph = -1;
//	S.push(root);
//	root = huffTable[0];
//	while (!S.empty()) {
//		if (root.left > 0) {
//			//block for stack so that it doesn't traverse the same way
//			blocker = root.left;
//			root.left = -2;
//			//add to stack
//			S.push(root);
//			//move left
//			root = huffTable[blocker];
//			bitString += "0";
//		}
//		else if (root.left < 0 && root.right > 0)
//		{
//			blocker = root.right;
//			root.right = -2;
//			S.push(root);
//			//move right
//			root = huffTable[blocker];
//			bitString += "1";
//		}
//		//if left and right are both -1, you've found a glyph. add the bitArray to the bitSet map.
//		else if (!S.empty()) {
//			if (root.glyph != 255) {
//				bitSets[root.glyph] = bitString;
//			}
//			bitString = bitString.substr(0, bitString.length() - 1);
//			root = S.top();
//			S.pop();
//		}
//	} 
//
//	//cout << endl << endl;
//	//for (auto i : bitSets) {
//	//	cout << i.first << " : ";
//	//	cout << i.second;
//	//	cout << endl;
//	//}
//
///*
//
//	auto stop = chrono::high_resolution_clock::now();
//	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
//	cout << duration.count() << endl;*/
//}
//
//
////writes header to the file
//void writeHeader(ofstream& fileOut, string& fileName, const multimap<int, unsigned char>& glyphTable) {/*
//	auto start = chrono::high_resolution_clock::now();*/
//	int fileNameSize = fileName.length();
//
//	int tableSize = (glyphTable.size() * 2)+1;
//	fileOut.write((char*)&fileNameSize, 4);
//	//cout << fileNameSize;
//	fileOut.write((char*)&fileName.front(), fileName.length());
//	//cout << fileName;
//	fileOut.write((char*)&tableSize, sizeof tableSize);
//	//cout << tableSize << endl;
//	/*
//	auto stop = chrono::high_resolution_clock::now();
//	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
//	cout << duration.count() << endl;*/
//}
//
////writes the huffman table to the file
//void writeTable(ofstream& fileOut, huffNode huffTable[], const int& tableSize) {
//	for (int i = 0; i < tableSize; i++) {
//		fileOut.write((char*)&huffTable[i].glyph, sizeof huffTable[i].glyph);
//		fileOut.write((char*)&huffTable[i].left, sizeof huffTable[i].left);
//		fileOut.write((char*)&huffTable[i].right, sizeof huffTable[i].right);
//	}
//}
//
//
////tries to write out the bitsets to the file
//void writeMessage(ifstream& fileIn, ofstream& fileOut, map<unsigned char, string>& bitSets, const string &fileName, const int &fileSize) {/*
//	auto start = chrono::high_resolution_clock::now();*/
//
//	unsigned char c = ' ';
//	unsigned char bits;
//	int cnt = 0;
//	fileIn.close();
//	fileIn.open(fileName, ios::in | ios::binary);
//	fileOut.close();
//	fileOut.open((fileName.substr(0, fileName.length() - 4)) + ".huf", ios::out | ios::binary | ios::app);
//	
//	bits = '\0';
//	for (int i = 0; i < fileSize - 1; i++) {
//		fileIn.read((char*)&c, sizeof c);
//		//set bits to bitset
//		for (int b = 0; b < bitSets[c].length(); b++) {
//			if (cnt < 8) {
//				if (bitSets[c][b] == '1') {
//					bits = bits | (unsigned char)pow(2.0, cnt);
//				}
//				cnt++;
//			}
//			//if byte is full, write to file and reset
//			else {
//				cnt = 0;
//				fileOut.write((char*)&bits, sizeof bits);
//				//cout << bits << endl;
//				bits = '\0';
//				if (bitSets[c][b] == '1') {
//					bits = bits | (unsigned char)pow(2.0, cnt);
//				}
//				cnt++;
//			}
//		}
//
//	}
//	//output eof
//	for (int b = 0; b < bitSets[256].length(); b++) {
//		if (cnt < 8) {
//			if (bitSets[256][b] == '1') {
//				bits = bits | (unsigned char)pow(2.0, cnt);
//			}
//			cnt++;
//		}
//		else {
//			cnt = 0;
//			fileOut.write((char*)&bits, sizeof bits);
//			//cout << bits << endl;
//			bits = '\0';
//			if (bitSets[256][b] == '1') {
//				bits = bits | (unsigned char)pow(2.0, cnt);
//			}
//			cnt++;
//		}
//	}
//
//	fileOut.write((char*)&bits, sizeof bits);
//	//cout << bits << endl;
//
//
//	/*
//	auto stop = chrono::high_resolution_clock::now();
//	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
//	cout << duration.count() << endl;*/
//}
////opens files, stupid
//void openFiles(ifstream &fileIn, ofstream &fileOut, const string &fileName, const int &fs) {
//	fileIn.open(fileName, ios::in | ios::binary);
//	if (!fileIn.is_open()) {
//		cout << "fileIn failed to open.\n";
//		fileIn.close();
//	}
//
//		string outFileName = fileName.substr(0, fileName.length() - 4);
//		outFileName = outFileName + ".huf";
//		fileOut.open(outFileName, ios::out | ios::binary);
//		if (!fileOut.is_open()) {
//			cout << "fileOut failed to open.\n";
//			fileOut.close();
//		}
//
//}
