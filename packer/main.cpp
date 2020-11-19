#include "packingInfo.h"

/********************************************************************
this prograom can only accepts one valid EXE files.
It will then pack them into one exe file, for example, calling it output.exe.
the output.exe will unpack the exe files and run them at the same time.

this program will only run with arguments in the terminal.
it will never ask for user input during runtime
it will only display if it is success or fail.
********************************************************************/

int main(int argc, char *argv[]){
    //display how to use this
    if (argc < 3){ //not enough arguments to proceed
        printf("hXOR Packer by Afif, 2012"
               "\n--------------------------------------------------------------------------\n"
               "How to use?\n\n"
               "For Packing:\n"
               "<S> -> EXE File (Absolute Path)\n"
               "<D> -> Destination Output (Absolute Path)\n"
               "<P> -> Parameters (Optional)\n"
               "<K> -> Xor Encryption Key in numbers (Optional)\n"
               "\nAvaliable Parameters (Optional):\n"
               "-c\t\tCompression\n-e\t\tEncryption\n-ce\t\tCompression & Encryption\n\nExamples:\n"
               ">>>packer.exe <S> <D> <P> <K>\n"
               ">>>packer.exe C:\\in.exe C:\\folder\\out.exe\n"
               ">>>packer.exe C:\\in.exe C:\\folder\\out.exe -ce 56213\n\n"
               );
    }

    if(argc >= 3){ //packing
        printf("hXOR Packer by Afif, 2012"
               "\n--------------------------------------------------------------------------\n");
        int resultError = packFileIntoArchive(argc, argv);

        if(resultError != PESuccess)
            printf("Error: %s \n", PEerrors_str[resultError]);
    }

    system("PAUSE");
    return 0;
}
