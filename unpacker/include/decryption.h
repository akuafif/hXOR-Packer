#ifndef DECRYPTION_H
#define DECRYPTION_H

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

UCHAR* decryptFile(UCHAR *input, long size);
UCHAR* decryptFile(UCHAR *input, long size, int key);

#endif // DECRYPTION_H
