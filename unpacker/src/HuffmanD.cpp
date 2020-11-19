#include "HuffmanD.h"

/********************************************************************
    The constructor for the HuffmanD object.
    It will set the trees array with empty node.
    And the allocatedoutput is NULL, so that the function getOutput()
    will return NULL/false if there is nothing being compressed.
*********************************************************************/
HuffmanD::HuffmanD(){
	stepscount = 0;
	treescount = 0;

	for (register int i = 0; i < 256; i++){
		*(trees+i)  = new node();
		*(trees_backup+i) = *(trees+i);
		*(leaves+i) = *(trees+i);
		*(STEPS+i)  = 0;
	}
	memset(nodes, 0, 256*sizeof(node));
	allocatedoutput = NULL;
}


/********************************************************************
    This function will decompress the file. It will first get the
    neccessary info on how to construct back the tree from the header
    of the Input file.
    This is how the structure of the Input file created by the
    compressor.
                        [ The number of trees       ]
                        [ Characters                ]
                        [ The deepness of the tree  ]
                        [ Output size               ]

*********************************************************************/
int HuffmanD::Decompress(UCHAR *input, int inputlength){ //input = file content, inputlenght = file size.
	UCHAR *stop = input + inputlength; //points to the last byte of file
	register UCHAR *inptr = input; //inptr will be used to navigate inside the array

	treescount = *inptr;  // get the treescount from the file header
	inptr++;
	treescount++; // trees count is always +1

	node **tptr = trees;
	int outsize;

	//gets all the char from the input file
	for (register int i = 0; i < treescount; i++){
		(*tptr)->chr = *inptr;
		inptr++; // go forward
		tptr++;
	}
	//printf("treescount: %d\n", treescount);

	stepscount = *inptr;
	//printf("stepscount: %d\n", stepscount);

	inptr++;
	int *sptr = STEPS;
	for (register int i = 0; i < stepscount; i++){
		(*sptr) = *inptr;
		inptr++;
		sptr++;
	}

	//printf("outsize = *inptr << 24: %d\n", *inptr << 24);
    //printf("outsize ^= *(inptr+1) << 16: %d\n", *(inptr+1) << 16);
    //printf("outsize ^= *(inptr+2) << 8: %d\n", *(inptr+2) << 8);
    //printf("outsize ^= *(inptr+3): %d\n", *(inptr+3));

    outsize  = *inptr << 24;  // xor
	outsize ^= *(inptr+1) << 16;
	outsize ^= *(inptr+2) << 8;
	outsize ^= *(inptr+3);
	inptr+=4;
    //printf("outsize: %d\n", outsize);

	allocatedoutput = new UCHAR[outsize+10]; // allocate output

	MakeHuffmanTree();

	setCodeAndLength(*trees, 0,0);  // initialize leaves - set their codes and code lengths

	register UCHAR *outptr = allocatedoutput;
	int bit = 0;
	register node *nptr ;
	register int b;
	while(inptr <= stop){  // decompress
		nptr = *trees; // root
		while(nptr->codelength == 0){
			b = ((*inptr) >> bit) &1;
			nptr = (b > 0) ? nptr->right :  nptr->left;
			inptr+=( (bit >> 2) & (bit >> 1) & bit) & 1;
			bit++;
			bit&=7;
		}
		(*outptr) = nptr->chr;
		outptr ++;
	}

	return outsize;
}

/********************************************************************
    Generate the big huffman tree with the nodes in the trees array

    In the while loop below,
    It will create a new node and store it in the nodes array,
    which stores the two nodes taken from the trees in reversed order
    and the sum of total count of its child.

    It will try to relocate the node that inside the tree to a better
    place.
*********************************************************************/
void HuffmanD::MakeHuffmanTree(){
node *n;
	node *nptr2 = nodes;
	node **backupptr;
	node **tptr = trees;
	stepscount = 0;
	while (treescount > 1)  // merge trees until only 1 remains
	{
		n = nptr2;
		nptr2++;
		tptr = trees+treescount;
		backupptr = trees_backup+treescount;

		n->right = *(tptr-2);
		n->left = *(tptr-1);

        //preparing pointers for the next loop
		*(tptr-1) = *(backupptr-1);
		*(tptr-2) = n;
		treescount--;

		moveTreesToRight(trees+(*(STEPS+stepscount)));  //move the nodes based on the frequency.
		stepscount++;
	}
}

/********************************************************************
    This function will set the code and path to the last leaf if
    each tree.
    For example: HUFFMAN
    Char    Count   Code    CodeLenght
    H       1       000     3
    U       1       100     3
    F       2       10      2
    M       1       01      2
    A       1       011     3
    N       1       111     3

               6
          0/       \1
          3         3
       0/  \       / \1
       2    \    0/   2
     0/ \1   \1  /  0/ \1
     H   U   F   M   A   N
*********************************************************************/
void HuffmanD::setCodeAndLength(node *n, int path, int level){
	bool leaf = true;
	if (n->right){
		setCodeAndLength(n->right, path ^ (1 << level), level+1);
		leaf = false;
	}
	if (n->left){
		setCodeAndLength(n->left, path, level+1);
		leaf = false;
	}
	if (leaf){ //the last leaf that is H,U,F,M,A,N
		n->codelength = level;
		n->code = path;
	}
}

/********************************************************************
    Exchange the location of the node in the parameter with
    the node at the end of the tree
*********************************************************************/
void HuffmanD::moveTreesToRight(node **toTree){
	node *a, *b;
	register node **ptr = trees+treescount-1;
	register node **_toTree = toTree;
	while (ptr > _toTree){
		a = *(ptr-1);
		b = *(ptr);
		*(ptr-1) = b;
		*(ptr) = a;
		ptr--;
	}
}

/********************************************************************
    return the output pointer
*********************************************************************/
UCHAR *HuffmanD::getOutput(){
	if (allocatedoutput)
		return allocatedoutput;
	return NULL;
}
