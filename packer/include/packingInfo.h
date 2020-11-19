#ifndef PACKINGINFO_H
#define PACKINGINFO_H

/********************************************************************
    #INCLUDE
*********************************************************************/
#include <windows.h> //mainly used for MAX_PATH, HANDLE and more
#include <stdio.h> //used for printf etc
#include <encryption.h> //encryption header
#include <huffman.h> //compression header
#include <string>

using std::string;

/********************************************************************
    TYPEDEF DEFINITION
*********************************************************************/
typedef struct {
   // char oriPath[MAX_PATH];
    char filename[MAX_PATH];
    long filesize;
    int key;
    int parameter;
} packdata_t;

/********************************************************************
    GLOBAL VARIABLES
*********************************************************************/
enum packingErrors{
    PESuccess = 0, //no error at all
    PEerrorNoFiles, //no files to extrace. invalid archive
    PEerrorPath, //error in path
    PEerrorCannotCreateArchive, //unable to create archive
    PEerrorCouldNotOpenArchive,
    PEerrorInvalidParameter,
    PEerrorInputNotEXE
};
extern const char *PEerrors_str[];

enum parameters{
    PREmpty = 0, //no valid parameter at all
    PRCompression = 1, //compression only
    PREncrpytion = 2, //encrytion only
    PRBoth
};
extern const char *Parameter_str[];
extern const char unpackerStub[];

/********************************************************************
    FUNCTION DECLARATION
*********************************************************************/
int packFileIntoArchive(int, char *[]);
int unpackFiles(char *, char *, long startPosition = 0);
int getInsertPosition(char *, long *);
int setInsertPosition(char *, long);
int validExeFile(const char *);

#endif // PACKINGINFO_H
