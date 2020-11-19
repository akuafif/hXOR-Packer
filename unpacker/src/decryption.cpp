#include "decryption.h"

UCHAR* decryptFile(UCHAR *input, long size){
    UCHAR *inptr, *outptr, *decryptInput;
    decryptInput = (UCHAR *) malloc (size*sizeof(UCHAR));

    inptr = input;
    outptr = decryptInput;
    char a;

    //generate key
    srand(size);
    int key = rand() % 69;
    printf("key is : %d\n", key);

    //encrypt using XOR key
    for(int cur = 0; cur < size; ++cur){
        a = *(inptr);
        int b = (int)a;
        b ^= key;
        a = char(b);
       *(outptr) = a;
        ++outptr;
        ++inptr;
    }

    return decryptInput;
}

UCHAR* decryptFile(UCHAR *input, long size, int key){
    UCHAR *inptr, *outptr, *decryptInput;
    decryptInput = (UCHAR *) malloc (size*sizeof(UCHAR));

    inptr = input;
    outptr = decryptInput;
    char a;

    //generate key
    printf("key is : %d\n", key);
    srand(key);
    key = rand() % 69;

    //encrypt using XOR key
    for(int cur = 0; cur < size; ++cur){
        a = *(inptr);
        int b = (int)a;
        b ^= key;
        a = char(b);
       *(outptr) = a;
        ++outptr;
        ++inptr;
    }

    return decryptInput;
}
