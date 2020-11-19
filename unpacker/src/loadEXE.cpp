/********************************************************************
    #INCLUDE
*********************************************************************/
#include "loadEXE.h"

/********************************************************************
    GLOBAL VARIABLES
*********************************************************************/
const char *PEerrors_str[] ={
    "Success",
    "Archive empty; no files to extract",
    "File in supplied path not found",
    "Not a valid packed file",
    "Failed while extracting one of the files",
    "Input archive file failed to open",
    "Unable to start a dummy process",
    "Packed file is not a valid executable file",
    "Unable to Write Process Memory"
};


/********************************************************************
    FUNCTION DEFINITION
*********************************************************************/
/********************************************************************
    This function will unpack the files inside you.
    It will then call the function LoadEXE(LPVOID) to
    run the EXE from memory
*********************************************************************/
int unpackFiles(char *binFile, long startPosition, char *pPath){
    //open binfile in binary to read
    FILE *packArchive = fopen(binFile, "rb");

    //not valid path to bin?
    if(!packArchive)
        return PEerrorCouldNotOpenArchive;

    //printf("startingPos: %l", startPosition);
    if (startPosition) //0 is false
        fseek(packArchive, startPosition, SEEK_SET);
    //startPosition = 0;

    long binSignature; //the variable name is too obvious
    packdata_t pdata;

    //read signature 'AFIF'
    //fread(ptr to store, size of each element, how many times to do this?, the file needs to be read)
    fread(&binSignature, sizeof(binSignature), 1, packArchive);
    if(binSignature != 'AFIF')
        return(fclose(packArchive), PEerrorNoSignatureFound);

    //read pdata in the bin file
    fread(&pdata, sizeof(packdata_t), 1, packArchive);

////////////////////////////////////////////////////////////////////////
    printf("Extracting >>>> %s [%li]\n", pdata.filename, pdata.filesize);

    //how big is the size to be written?
    long size = pdata.filesize;

    //preparing variables for decryption and/or decompression
    UCHAR *content, *decryptedContent, *output;
    int outsize;
    HuffmanD *huf;

    content = (UCHAR *) malloc (size*sizeof(UCHAR));

    fread(content, size, 1, packArchive);
    fclose(packArchive);

    //check if user provided the key or is packer generated.
    bool keyProvided = 0;
    if(pdata.key != 0)
        keyProvided = 1;

/********************************************************************
    Decrypt and/or decompress the content
*********************************************************************/
    switch(pdata.parameter){
    case 0: //none
        outsize = pdata.filesize;
        decryptedContent = content;
    break;
    case 1: //decompression
        printf("\nDecompressing >>>> %s \n", pdata.filename);
        huf = new HuffmanD();
        outsize = huf->Decompress(content, pdata.filesize);

        output = huf->getOutput();
        decryptedContent = output;
    break;
    case 2: //decryption
        printf("\nDecrypting >>>> %s \n", pdata.filename);

        //allocate array for decrypted content
        decryptedContent = (UCHAR *) malloc (size*sizeof(UCHAR));
        if(keyProvided == 0)
            decryptedContent = decryptFile(content, size);
        else
            decryptedContent = decryptFile(content, size, pdata.key);
        outsize = pdata.filesize;
    break;
    case 3: //both
        //decompressing
        printf("\nDecompressing >>>> %s \n", pdata.filename);
        huf = new HuffmanD();
        outsize = huf->Decompress(content, pdata.filesize);

        output = huf->getOutput();

        //allocate array for decrypted content
        decryptedContent = (UCHAR *) malloc (outsize*sizeof(UCHAR));

        //decrypting
        printf("\nDecrypting >>>> %s \n", pdata.filename);
        if(keyProvided == 0)
            decryptedContent = decryptFile(output, outsize);
        else
            decryptedContent = decryptFile(output, outsize, pdata.key);
    break;
    }

    printf("\nUnpacking Successful!\n\nExecuting from Memory >>>> %s [%i]\n", pdata.filename, outsize);
    LoadEXE(decryptedContent);

    return PESuccess;
}

/********************************************************************
    This function will dynamically fork a process.
    lpImage contains the data/image of the EXE.
*********************************************************************/
int LoadEXE(LPVOID lpImage){
/********************************************************************
    Variables for Process Forking
*********************************************************************/
    ULONG lWritten;
    ULONG lHeaderSize;
    ULONG lImageSize;
    ULONG lSectionCount;
    ULONG lSectionSize;
    ULONG lFirstSection;
    ULONG lPreviousProtection;
    ULONG lJumpSize;

    LPVOID lpImageMemory;
    LPVOID lpImageMemoryDummy;

    IMAGE_DOS_HEADER dsDosHeader;
    IMAGE_NT_HEADERS ntNtHeader;
    IMAGE_SECTION_HEADER shSections[512 * 2];

    PROCESS_INFORMATION piProcessInformation;
    STARTUPINFO suStartUpInformation;

    CONTEXT cContext;

    NtUnmapViewOfSectionF NtUnmapViewOfSection = (NtUnmapViewOfSectionF)GetProcAddress(LoadLibrary("ntdll.dll"),"NtUnmapViewOfSection");
/********************************************************************
    Variables for Creating a Local Process
*********************************************************************/
    FILE* dummyInFile;
    char* dummyProcessName;

    long int dummySize;
    long int dummyImageBase;
    long int dummyImageSize;

    LPVOID dummyFile;

    IMAGE_DOS_HEADER dummyDosHeader;
    IMAGE_NT_HEADERS dummyNtHeader;
/********************************************************************
    Create and prepare a dummy process for Forking
*********************************************************************/
    dummyProcessName = new char[MAX_PATH];
    //fills the dummyProcessName with zeros
    ZeroMemory(dummyProcessName,MAX_PATH);

    // Get the file name for the dummy process
    if(GetModuleFileName(NULL,dummyProcessName,MAX_PATH) == 0){
        delete [] dummyProcessName;
        return PEerrorDummyProcessFail;
    }

    // Open the dummy process in binary mode
    dummyInFile = fopen(dummyProcessName,"rb");
    if(!dummyInFile){
        delete [] dummyProcessName;
        return PEerrorDummyProcessFail;
    }

    fseek(dummyInFile,0,SEEK_END);

    // Get file size
    dummySize = ftell(dummyInFile);
    //printf("local file size of dummyProcessName : %lu\n", dummySize);

    rewind(dummyInFile);

    // Allocate memory for dummy file and fill it with zeros
    dummyFile = new LPVOID[dummySize];
    ZeroMemory(dummyFile,dummySize);

    // Read all the zero memory of file
    fread(dummyFile,dummySize,1,dummyInFile);

    // Close file
    fclose(dummyInFile);

    // Grab the DOS Headers
    memcpy(&dummyDosHeader,dummyFile,sizeof(dummyDosHeader));

    // Checking for dos signature
    if(dummyDosHeader.e_magic != IMAGE_DOS_SIGNATURE){
        delete [] dummyProcessName;
        delete [] dummyFile;
        return PEerrorInputNotEXE;
    }

    // Grab NT Headers
    memcpy(&dummyNtHeader,(LPVOID)((long int)dummyFile+dummyDosHeader.e_lfanew),sizeof(dummyDosHeader));

    // Checking for NT sigmature
    if(dummyNtHeader.Signature != IMAGE_NT_SIGNATURE){
        delete [] dummyProcessName;
        delete [] dummyFile;
        return PEerrorInputNotEXE;
    }

    // Deallocate
    delete [] dummyFile;

    // Get Size and Image Base
    dummyImageBase = dummyNtHeader.OptionalHeader.ImageBase;
    dummyImageSize = dummyNtHeader.OptionalHeader.SizeOfImage;
/********************************************************************
    Getting all required data from the EXE inside you
    to prepare for Forking Process.
*********************************************************************/
    // Grabbing DOS Header from the EXE inside you
    memcpy(&dsDosHeader,lpImage,sizeof(dsDosHeader));

    if(dsDosHeader.e_magic != IMAGE_DOS_SIGNATURE){
        delete [] dummyProcessName;
        return PEerrorInputNotEXE;
    }

    // Grabbing DOS Header from the EXE inside you
    memcpy(&ntNtHeader,(LPVOID)((long int)lpImage+dsDosHeader.e_lfanew),sizeof(ntNtHeader));

    if(ntNtHeader.Signature != IMAGE_NT_SIGNATURE){
        delete [] dummyProcessName;
        return PEerrorInputNotEXE;
    }

    // Getting the proper sizes
    lImageSize = ntNtHeader.OptionalHeader.SizeOfImage;
    lHeaderSize = ntNtHeader.OptionalHeader.SizeOfHeaders;

    // Allocatting memory for the EXE image
    lpImageMemory = new LPVOID[lImageSize];
    ZeroMemory(lpImageMemory,lImageSize);

    lpImageMemoryDummy = lpImageMemory;

    // Getting the sections
    lFirstSection = (long int)(((long int)lpImage+dsDosHeader.e_lfanew) + sizeof(IMAGE_NT_HEADERS));

    memcpy(shSections,(LPVOID)(lFirstSection),sizeof(IMAGE_SECTION_HEADER)*ntNtHeader.FileHeader.NumberOfSections);
    memcpy(lpImageMemoryDummy,lpImage,lHeaderSize);

    // Get Section Alignment using Modulo
    if((ntNtHeader.OptionalHeader.SizeOfHeaders % ntNtHeader.OptionalHeader.SectionAlignment) == 0){
        lJumpSize = ntNtHeader.OptionalHeader.SizeOfHeaders;
    }else{
        lJumpSize  = (ntNtHeader.OptionalHeader.SizeOfHeaders/ntNtHeader.OptionalHeader.SectionAlignment);
        lJumpSize += 1;
        lJumpSize *= (ntNtHeader.OptionalHeader.SectionAlignment);
    }

    lpImageMemoryDummy = (LPVOID)((long int)lpImageMemoryDummy + lJumpSize);

    // Copy Sections To Buffer
    for(lSectionCount = 0; lSectionCount < ntNtHeader.FileHeader.NumberOfSections; lSectionCount++){
        lJumpSize = 0;
        lSectionSize = shSections[lSectionCount].SizeOfRawData;

        memcpy(lpImageMemoryDummy,(LPVOID)((long int)lpImage + shSections[lSectionCount].PointerToRawData),lSectionSize);

        if((shSections[lSectionCount].Misc.VirtualSize % ntNtHeader.OptionalHeader.SectionAlignment)==0){
            lJumpSize = shSections[lSectionCount].Misc.VirtualSize;
        }else{
            lJumpSize  = (shSections[lSectionCount].Misc.VirtualSize/ntNtHeader.OptionalHeader.SectionAlignment);
            lJumpSize += 1;
            lJumpSize *= (ntNtHeader.OptionalHeader.SectionAlignment);
        }

        lpImageMemoryDummy = (LPVOID)((long int)lpImageMemoryDummy + lJumpSize);
    }

    ZeroMemory(&suStartUpInformation,sizeof(STARTUPINFO));
    ZeroMemory(&piProcessInformation,sizeof(PROCESS_INFORMATION));
    ZeroMemory(&cContext,sizeof(CONTEXT));

    suStartUpInformation.cb = sizeof(suStartUpInformation);
/********************************************************************
Writing the lpImageMemory into a new Process.
 -  Create a Process in suspended mode to fork.
 -  Changes the protection of virtual address space of the Process.
 -  Reserves or commits a region of memory within the virtual
    address space of the Process, setting it all to zeros.
 -  Writes lpImageMemory into the Process.
*********************************************************************/
    if(CreateProcess(NULL,dummyProcessName,NULL,NULL,false,CREATE_SUSPENDED,NULL,NULL,&suStartUpInformation,&piProcessInformation)){
        cContext.ContextFlags = CONTEXT_FULL;
        GetThreadContext(piProcessInformation.hThread,&cContext);

        // Check image base and image size
        if(dummyImageBase == (long int)ntNtHeader.OptionalHeader.ImageBase && lImageSize <= dummyImageSize)
        {
            VirtualProtectEx(piProcessInformation.hProcess,(LPVOID)((long int)ntNtHeader.OptionalHeader.ImageBase),lImageSize,PAGE_EXECUTE_READWRITE,(unsigned long*)&lPreviousProtection);
        }else{
            if(!NtUnmapViewOfSection(piProcessInformation.hProcess,(LPVOID)((DWORD)dummyImageBase)))
                VirtualAllocEx(piProcessInformation.hProcess,(LPVOID)((long int)ntNtHeader.OptionalHeader.ImageBase),lImageSize,MEM_COMMIT | MEM_RESERVE,PAGE_EXECUTE_READWRITE);
        }

        // Write Image to Process
        if(!(WriteProcessMemory(piProcessInformation.hProcess,(LPVOID)((long int)ntNtHeader.OptionalHeader.ImageBase),lpImageMemory,lImageSize,(unsigned long*)&lWritten))){
            delete [] dummyProcessName;
            delete [] lpImageMemory;
            return PEerrorWriteProcessFail;
        }

        // Set Image Base
        if(!(WriteProcessMemory(piProcessInformation.hProcess,(LPVOID)((long int)cContext.Ebx + 8),&ntNtHeader.OptionalHeader.ImageBase,4,(unsigned long*)&lWritten))){
            delete [] dummyProcessName;
            delete [] lpImageMemory;
            return PEerrorWriteProcessFail;
        }
/********************************************************************
    Setting a new entry point. And Resume the Process
*********************************************************************/
        cContext.Eax = ntNtHeader.OptionalHeader.ImageBase + ntNtHeader.OptionalHeader.AddressOfEntryPoint;

        SetThreadContext(piProcessInformation.hThread,&cContext);

        if(dummyImageBase == (long int)ntNtHeader.OptionalHeader.ImageBase && lImageSize <= dummyImageSize)
            VirtualProtectEx(piProcessInformation.hProcess,(LPVOID)((long int)ntNtHeader.OptionalHeader.ImageBase),lImageSize,lPreviousProtection,0);

        // Resume the process
        ResumeThread(piProcessInformation.hThread);
    }

    delete [] dummyProcessName;
    delete [] lpImageMemory;

    return PESuccess;
}

/********************************************************************
    Get the e_res value which stores the starting
    position of the packed content
*********************************************************************/
int getInsertPosition(char *filename, long *pos){
    FILE *packArchive = fopen(filename, "rb");
    if (packArchive == NULL)
        return PEerrorCouldNotOpenArchive;

    IMAGE_DOS_HEADER idh;

    fread((void *)&idh, sizeof(idh), 1, packArchive);
    fclose(packArchive);
    *pos = *(long *)&idh.e_res2[0];
    return PESuccess;
}
