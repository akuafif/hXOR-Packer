#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <windows.h>

class HuffmanD{
private:
	class node{
	public:
		node *left, *right;
		int count;       // frequency
		int code;        // the code or path to it. eg, 11010
		int codelength;  // the number of bits taken. eg, 11010 is 5 bits
		UCHAR chr;        // the actual character is being compress
		node(void)  {memset(this, 0, sizeof(node));} // constructor
		~node() {
			if (left) {delete left; left = NULL;}
			if (right) {delete right; right = NULL;}
		}
	};
	node *trees[256];  // Array of trees,
	node *leaves[256]; // array of leaves, contains ASCII char that being compressed
	                   // ref. count, later code and codelength too. It is a reference to original trees above
	node *trees_backup[256];
	int STEPS[256];    // as the tree is constructed, this stores exact steps (movings) of the last tree
	int stepscount;
	node nodes[256];

	void MakeHuffmanTree();

	int treescount;
	UCHAR *allocatedoutput;
	void setCodeAndLength(node *, int, int); // set code and codelength of the leaves
	void moveTreesToRight(node **toTree);

public:
	HuffmanD();
	int Decompress(UCHAR *input, int inputlength);

	UCHAR *getOutput(); // get the actual decompreess data
	int getLastError();
};

#endif
