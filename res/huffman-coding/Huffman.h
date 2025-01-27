#ifndef SVDCOMPRESSION_HUFFMAN_H
#define SVDCOMPRESSION_HUFFMAN_H

#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <stdio.h>
#include <cstdlib>
#include <algorithm>
using namespace std;

struct Tree
{
    int frequency;
    unsigned char character;
    Tree* left = NULL;
    Tree* right = NULL;
};

class Huffman {
public:
    bool operator()(Tree* a, Tree* b)
    {
        return a->frequency > b->frequency;
    }
};

Tree* buildHuffmanTree(vector<pair<unsigned char, int> > freqTable);
string toBinary(unsigned  char a);
vector<pair<unsigned char, int> > convertToVector(map<unsigned char, int> codes);
string getHuffmanBitstring(unsigned char* buffer, map<unsigned char, string> codes, int sz, int& paddedBits);
unsigned char* getBufferFromString(string bitstring, vector<unsigned char>& outputBuffer, int& sz);
string getStringFromBuffer(unsigned char* buffer, int sz);
unsigned char* getDecodedBuffer(string bitstring, vector<unsigned char>& buffer, map<unsigned char, string> codes, int& sz, int paddedBits);
void compressFile(const char* path,const char* output_path, map<unsigned char, string>& codes);
void decompressFile(const char* inputPath, const char* outputPath);
int CompareFiles(const char* pFname1,const char* pFname2);


#endif //SVDCOMPRESSION_HUFFMAN_H
