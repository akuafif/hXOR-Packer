#include "loadEXE.h"

int main(int argc, char *argv[]){
    if(argc > 1 && string(argv[1]) == "-ls"){
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
        freopen("CONOUT$", "w", stderr);
        freopen("CONIN$", "r", stdin);
    }

    if(!isAll()){
        char packArchive[MAX_PATH];
        long startingPosition;

        // get self file name by using GetModuleName
        // how GetModuleFileName works?
        GetModuleFileName(NULL,                 // in HMODULE hModule. If NULL, current process path will be used
                          packArchive,          // out LPTSTR lpFilename. Variable to capture
                          sizeof(packArchive)); //in DWORD nSize.

         // get the starting position of the BIN archive inside you
        getInsertPosition(packArchive, &startingPosition);

        printf("hXOR Un-Packer by Afif, 2012"
               "\n--------------------------------------------------------------------------\n");

        int rc = unpackFiles(packArchive, startingPosition, argv[0]);
        if (rc != PESuccess)
            printf("%s \n", PEerrors_str[rc]);
        else
            printf("\nDone!\n");
    }

    if(argc > 1 && string(argv[1]) == "-ls")
        system("PAUSE");

    return 0;
}
