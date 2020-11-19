#include "encryption.h"

UCHAR* encryptFile(UCHAR *input, long size){
    UCHAR *inptr, *outptr, *encryptInput;
    inptr = input;

    encryptInput = new UCHAR[size];
    outptr = encryptInput;
    char a;

    //generate key
    srand(size);
    int key = rand() % 69;
    printf("key is : %d\n", key);

    //encrypt using XOR key
    for(long cur = 0; cur < size; ++cur){
        a = *(inptr);
        int b = (int)a;
        b ^= key;
        a = char(b);
       *(outptr) = a;
        ++outptr;
        ++inptr;
    }

    //return encrypted content
    return encryptInput;
}

UCHAR* encryptFile(UCHAR *input, long size, int key){
    UCHAR *inptr, *outptr, *encryptInput;
    inptr = input;

    encryptInput = new UCHAR[size];
    outptr = encryptInput;
    char a;

    //generate key
    printf("key is : %d\n", key);

    //make sure the xor result will not more that the acsii table value.
    srand(key);
    key = rand() % 69;

    //encrypt using XOR key
    for(long cur = 0; cur < size; ++cur){
        a = *(inptr);
        int b = (int)a;
        b ^= key;
        a = char(b);
       *(outptr) = a;
        ++outptr;
        ++inptr;
    }

    //return encrypted content
    return encryptInput;
}

