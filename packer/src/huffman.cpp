#include "huffman.h"

/********************************************************************
    The constructor for the huffman object.
    It will set the trees array with empty node.
    And the allocatedoutput is NULL, so that the function getOutput()
    will return NULL/false if there is nothing being compressed.
*********************************************************************/
huffman::huffman(){
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
    This function will compress the unsigned char array that is
    being pass in in the parameter. You can get the compressed
    version of the unsigned char array by calling the getOutput()
    function.
    This function will return the compressed size.
*********************************************************************/
int huffman::Compress(UCHAR *input, int inputlength){ //input = file content, inputlenght = file size.
	register UCHAR *inptr = input;
	UCHAR *stop = input+inputlength;  // ptr to the end of input file
	allocatedoutput = new UCHAR[5*inputlength];

    allocatedoutput = allocatedoutput;
	int byteswritten = 0;
	register UCHAR *outptrX = allocatedoutput;

/********************************************************************
    Get the Count for each Character. Store it in the trees array
*********************************************************************/
    //printf("Char\tCount\n");
	while (inptr != stop){
        //ptr to location of array of nodes-> variable
		(*(trees+*inptr))->count++;
		(*(trees+*inptr))->chr = (*inptr);

        //printf("%c\t%d\n", (*inptr), (*(trees+*inptr))->count);
		inptr++;
	}

/********************************************************************
    Sort the arrays by the highest count/frequency first
*********************************************************************/
	qsort(trees, 256, sizeof(node*), compareFrequency);

/********************************************************************
    Write down treescount at the begining of the output file
    (for decompression)
*********************************************************************/
    for (register int i = 0; i < 256; i++) // get the trees count
		if ((*(trees+i))->count > 0)
			treescount++;
		else break;

    //printf("\ntresscount:%d\n", treescount);
    (*outptrX) = treescount-1;
    ++outptrX;

/********************************************************************
    Write down all the characther exist in the tree. Only one of it.
    NO DUPLICATIONS.
*********************************************************************/
    for (register int i = 0; i < treescount; i++){
        *outptrX = (*(trees+i))->chr;
        //printf("%c\t%c \n", (*(outptrX)), (*(trees+i))->chr);
        ++outptrX;
    }

/********************************************************************
    Make Huffman Tree and write the stepscounts to outPtrX
*********************************************************************/
    MakeHuffmanTree();

    // write steps
    //printf("stepscount:%d\n", stepscount);
    *outptrX = stepscount;
    outptrX++;

    for (register int i = 0; i < stepscount; i++){
        *outptrX = *(STEPS+i);
        outptrX++;
    }

/********************************************************************
    Write the original input file size.
    By using BitShift, it helps to decrese the size to store it.
*********************************************************************/
    //printf("inputlength >> 24: %d\n", inputlength >> 24);
    //printf("inputlength >> 16: %d\n", inputlength >> 16);
    //printf("inputlength >> 8: %d\n", inputlength >> 8);
    //printf("inputlength: %d\n", inputlength);

    *outptrX     =  inputlength >> 24;  // write original stream's length
    *(outptrX+1) =  inputlength >> 16;
    *(outptrX+2) =  inputlength >> 8;
    *(outptrX+3) =  inputlength;
    outptrX+=4;

/********************************************************************
    Create leaves with the proper code and codelength
*********************************************************************/
    setCodeAndLength(*trees, 0,0); // assign code and codelenght to the leaf

    // compression
    int bitpath, bitswritten, outbitswritten = 0;
    register int i, bitcount;

/********************************************************************
    Writing the code and code length to each char into outPtrX
*********************************************************************/
    inptr = input; //inptr pointing to the start of the EXE file
    *outptrX = 0;
    while (inptr != stop){
        bitpath = (*(leaves+*inptr))->code;
        bitcount = (*(leaves+*inptr))->codelength;
        bitswritten =0;
        for (i = 0; i < bitcount; i++){
            //printf("((bitpath >> bitswritten)&1) << outbitswritten: %d\n", ((bitpath >> bitswritten)&1) << outbitswritten);
            (*outptrX) ^= ((bitpath >> bitswritten)&1) << outbitswritten;
            bitswritten++;
            (*(outptrX+1)) = '\0'; //null ptr
            outptrX += ((outbitswritten >> 2) & (outbitswritten >> 1) & outbitswritten) &1;
            outbitswritten++; // bytes written growing with each 8 written bits ^^
            outbitswritten&=7;
        }
        inptr++;
    }

/********************************************************************
    Get the amount of bytes written. The size of the
    Compressed data.
*********************************************************************/
    byteswritten = outptrX - allocatedoutput;
    if (bitswritten > 0) // if necessary, increase the bytes written
        byteswritten++;   // e.g. if written 1 bit, even 1 bit needs whole byte
    return byteswritten;
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
void huffman::MakeHuffmanTree(){
	register node *n;
	register node **endtree = trees+treescount-1; //pointer to end of trees
	memset(nodes, 0, sizeof(node)*256);
	node *nptr = nodes;
	node **backupptr = trees_backup+treescount-1;
	while (treescount > 1){ // while trees count is > than > 1
		n = nptr; //ptr to new node from Nodes array
		nptr++;

		n->right = *(endtree-1);    //right is usually hold the higher frequency
		n->left = *endtree;         //left is usually the lesser frequency

		// the sum of total count from both left and right node are summed and assigned to new tree
		n->count = n->right->count + n->left->count;

        //assigning pointers to prepare for the next loop
		*endtree = *backupptr;
		*(endtree-1) = n;
		--treescount;
		--endtree;
		--backupptr;
		*(STEPS+stepscount) = treescount;

		//try to relocate the nodes in the trees array
		tryToRelocate();
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
void huffman::setCodeAndLength(node *n, int path, int level){
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
    Try to move the all nodes to a better location in the tree
    based on the frequency.
*********************************************************************/
void huffman::tryToRelocate(){
	register node **ptr = trees+treescount-2;
	register node **ourtree = trees+treescount-1;
	register node **begin = trees;
	while(ptr >= begin){
		if (ourtree != ptr)
			if (((*ptr)->count > (*ourtree)->count) || (ptr == begin)){
				moveTreesToRight(ptr);
				return;
			}
			ptr--;
	}
}

/********************************************************************
    Exchange the location of the node in the parameter with
    the node at the end of the tree
*********************************************************************/
void huffman::moveTreesToRight(node **toTree){
	node *a, *b;
	register node **ptr = trees+treescount-1;
	register node **_toTree = toTree;
	node **end = trees+treescount;
	while (ptr > _toTree){
		a = *(ptr-1);
		b = *(ptr);
		if (a->count >= b->count) {
			*(STEPS+stepscount) = treescount-(end-ptr);
			return;
		}
		*(ptr-1) = b;
		*(ptr) = a;
		ptr--;
	}
	*(STEPS+stepscount) = treescount-(end-ptr);
}


/********************************************************************
    return the output pointer
*********************************************************************/
UCHAR *huffman::getOutput(){
	if (allocatedoutput)
		return allocatedoutput;
	return NULL;
}

/********************************************************************
    This function will sort the trees array in the highest
    frequency first
*********************************************************************/
int huffman::compareFrequency(const void *A, const void *B){ // sorting, we want to sort descending
	if((*(node**)A)->count == (*(node**)B)->count)     // A->count == B->count ?
		return 0;
	 return (*(node**)A)->count < (*(node**)B)->count ? 1 : -1;// no? so if A->count < B->count give 1 else -1
}
