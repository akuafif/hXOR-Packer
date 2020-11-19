#ifndef LOADEXE_H
#define LOADEXE_H

/********************************************************************
    #INCLUDE
*********************************************************************/
#include "HuffmanD.h"
#include "decryption.h"
#include "antiDefense.h"
#include <stdio.h>
#include <windows.h>

/********************************************************************
    GLOBAL VARIABLES
*********************************************************************/
enum packingErrors{
    PESuccess = 0,                  //no error at all
    PEerrorNoFiles,                 //no files to extrace. invalid archive
    PEerrorPath,                    //error in path
    PEerrorNoSignatureFound,        //invalid archive, unable to unpack non-archive file
    PEerrorExtractError,            //cound not extract the archive file
    PEerrorCouldNotOpenArchive,     //fail to open archive for extraction
    PEerrorDummyProcessFail,        //cannot create dummy process
    PEerrorInputNotEXE,             //No valid MZ or NT header found
    PEerrorWriteProcessFail
}; extern const char *PEerrors_str[];

/********************************************************************
    TYPEDEF DEFINITION
*********************************************************************/
typedef long int (__stdcall* NtUnmapViewOfSectionF)(HANDLE,PVOID);

typedef struct {
    char filename[MAX_PATH];
    long filesize;
    int key;
    int parameter;
} packdata_t;

/********************************************************************
    FUNCTION DECLARATION
*********************************************************************/
int unpackFiles(char *binFile, long startPosition, char *pPath);
int getInsertPosition(char *filename, long *pos);
int LoadEXE(LPVOID lpImage);

#endif // LOADEXE_H
