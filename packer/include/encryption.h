#ifndef ENCRYPTION_H
#define ENCRYPTION_H


#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

UCHAR* encryptFile(UCHAR *input, long size);
UCHAR* encryptFile(UCHAR *input, long size, int key);
#endif // ENCRYPTION_H
