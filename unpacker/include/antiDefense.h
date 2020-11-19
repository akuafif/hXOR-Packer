#ifndef ANTIDEFENSE_H
#define ANTIDEFENSE_H

#include <stdio.h> //used for printf etc
#include <windows.h>
#include <string>
#include <TlHelp32.h>

using std::string;

bool runtimeDelay();
bool InSandboxie();
bool InVMware();
bool isProcessRunning(const string exeName);
bool isAll();

#endif // ANTIDEFENSE_H
