#include "antiDefense.h"

bool detected = 0;

bool runtimeDelay(){
    DWORD count, count2;

    printf("Scanning for execution delay...\n");

    count = GetTickCount();
    Sleep(500);
    count2 = GetTickCount();
    printf("count: %lu\ncount2: %lu\noffset: %lu\n\n", count, count2, count2-count);

    if ((count2 - count) > 550){
        detected = 1;
        return 1;
    }

    return 0;
}

bool InSandboxie(){
    printf("Scanning for Sandboxie...\n");
    if(GetModuleHandle("SbieDll.dll")){
        detected = 1;
        return 1;
    }

    return 0;
}

bool InVMware(){
    printf("Scanning for VMware...\n");
    if(isProcessRunning("VMwareUser.exe"))
    {
        detected = 1;
        return 1;
    }
    return 0;
}

bool isProcessRunning(const string exeName){
    PROCESSENTRY32 pce = {sizeof(PROCESSENTRY32)};
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);

    if(Process32First(hSnapshot, &pce))
    {

        do
        {

            if(!strcmp((const char*)pce.szExeFile, (const char*)exeName.c_str()))
            {
            return 1;
            }

        }while( Process32Next(hSnapshot, &pce) );

    }

    return 0;
}

bool isAll(){
    if(runtimeDelay())
        printf("Runtime scanning was detected\n\n");
    else if(InSandboxie())
        printf("Sandboxie detected!!!\n\n");
    else if(InVMware())
        printf("VMware detected!!!\n\n");

    if(detected==0)
        printf("Nothing was detected!\n\n");

    if(detected == 0) return 0;
    else return 1;
}
