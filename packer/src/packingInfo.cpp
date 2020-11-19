/********************************************************************
    #INCLUDE
*********************************************************************/
#include "packingInfo.h"

/********************************************************************
    GLOBAL VARIABLES
*********************************************************************/
const char *PEerrors_str[] =
{
    "Success",
    "Archive empty; no files to extract",
    "File in supplied path not found",
    "Could not create output archive file",
    "Failed to open one of the files",
    "Invalid Paramter",
    "Input file is not a valid executable file"
};

const char *Parameter_str[] =
{
    "None",
    "Compression Selected",
    "Encrytion Selected",
    "Compression and Encryption Selected"
};

/********************************************************************
    this is the unpackerstub. it will be added together with
    the BIN archive to make it into a self-extracting exe file
*********************************************************************/
const char unpackerStub[] = "unpackerLoadEXE.exe";

/********************************************************************
    FUNCTION DEFINITION
*********************************************************************/

/********************************************************************
    This function will pack the input EXE file into one
    self-executable file.

    This is how the structure of the exe file look like st the end
       [ Unpacker stub  ]
       [ BIN signature  ]
       [ pdata          ]
       [ Image EXE      ]
*********************************************************************/
int packFileIntoArchive(int count, char *argv[]){
    char *sPath = argv[1], *dPath = argv[2];


/********************************************************************
    pdata is a structure that holds the filename and size.
*********************************************************************/
    packdata_t pdata;

    printf("Input Path: %s "
           "\nOutput Path: %s \n\n",
           sPath, dPath);

    //check if the input path is valid
    if(GetFileAttributes(sPath) == INVALID_FILE_ATTRIBUTES)
        return PEerrorPath; //fail to go there

    //checking if the input file is a valid EXE
    if(validExeFile(sPath) == PEerrorInputNotEXE)
        return PEerrorInputNotEXE;

    //store files headers. name and size
    HANDLE hFile = CreateFile(sPath, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE,
                              0, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, 0);
    DWORD fileSize = GetFileSize(hFile, NULL);
    CloseHandle(hFile);

/********************************************************************
    INSERTING FILE INFO INTO pdata
*********************************************************************/
    char Path[MAX_PATH];
    char *filename[4096];

    GetFullPathName(sPath, 4096, Path, filename);

    // clear data to be safe
    memset(&pdata, 0, sizeof(pdata));

    //fill packdata entry with file name and size filesizelow
    strcpy(pdata.filename, *filename);
    pdata.filesize = fileSize;

/********************************************************************
                CREATING THE OUTPUT EXE FILE
*********************************************************************/
/********************************************************************
    1. Check for unpackerstub if in its your root folder
*********************************************************************/
    //does the unpacker stub exist in same directory as you?
    if(GetFileAttributes(unpackerStub) == INVALID_FILE_ATTRIBUTES){
        printf("Unpacker stub exe not found!\n");
        return 0;
    }

    //append .exe if the user didnt put it
    if (dPath[strlen(dPath)-1] != 'e')
        strcat(dPath, ".exe");

/********************************************************************
    2. Copy the stub into a new EXE file
*********************************************************************/
    //CopyFile( existing, new, false) false = haven't create the new file
    if (!CopyFile(unpackerStub, dPath, FALSE)){ //if return fail or 0
        printf("Could not create SFX file!\n");
        return PEerrorCannotCreateArchive;
    }

    //open the exe file for updating in binary
    FILE *packedEXE = fopen(dPath, "rb+");
    //FILE *packedEXE = fopen(dPath, "wb");

    //seek the last location
    fseek(packedEXE, 0, SEEK_END);

/********************************************************************
    3. Create a signature
*********************************************************************/
    //capture and store it
    long stubSize = ftell(packedEXE);

    //write signature . for the unpacker to identify if this is a valid archive for him
    long myPEsignature = 'AFIF';

    //how fwrite works?
    //fwrite(pointer to input, size to be written, nember of elements, output file)
    fwrite(&myPEsignature, sizeof(myPEsignature), 1, packedEXE);

/********************************************************************
    4. Reading the command line parameters
*********************************************************************/
    int parameter = 0;

    if(count == 3)
        parameter = PREmpty;
    else if(string(argv[3]) == "-c")
        parameter = PRCompression;
    else if(string(argv[3]) == "-e")
        parameter = PREncrpytion;
    else if(string(argv[3]) == "-ce")
        parameter = PRBoth;
    else
        return PEerrorInvalidParameter;

    //get the key for the XOR encryption if provided
    int key = 0;
    bool keyProvided = 0;
    if(count == 5){
        key = atoi(argv[4]); //will return 0 if its not a number in the argv[4]
        if(key == 0){
            return PEerrorInvalidParameter;
        }
        keyProvided = 1;
    }
/********************************************************************
    5. Encypt and/or Compress the input file
*********************************************************************/
    FILE *inFile = fopen(sPath, "rb");
    huffman *huf = new huffman();
    UCHAR *input, *output, *encryptedInput;
    long size;
    int outSize;

    //get the size
    size = pdata.filesize;
    input = new UCHAR[size];
    encryptedInput = new UCHAR[size];

    //fill the array with the content inside the file.
    fread(input, 1, size, inFile);

    printf("Option: %s\n", Parameter_str[parameter]);
    switch(parameter){
    case 0:
        outSize = size;
        output = input;
        break;
    case 1: //compression only
        printf("\nCompressing >>>> '%s' [%lu]\n", pdata.filename, pdata.filesize);
        outSize = huf->Compress(input, size);
        printf("Compressed Size: %d\n",outSize);

        output = huf->getOutput();
        break;
    case 2: //encryption only
        printf("\nEncrypting >>>> '%s'\n", pdata.filename);
        //call the compress function
        if(keyProvided == 0)
            output = encryptFile(input, size);
        else
            output = encryptFile(input, size, key);
        outSize = size;
        break;
    case 3://both
        printf("\nEncrypting >>>> '%s'\n", pdata.filename);

        //call the compress function
        if(keyProvided == 0)
            encryptedInput = encryptFile(input, size);
        else
            encryptedInput = encryptFile(input, size, key);

        //call compress function
        printf("\nCompressing >>>> '%s' [%lu]\n", pdata.filename, pdata.filesize);
        outSize = huf->Compress(encryptedInput, size);
        printf("Compressed Size: %d\n",outSize);

        output = huf->getOutput();
        break;
    }

/********************************************************************
    4. Copy the pdata and the compressed data
*********************************************************************/
    printf("\nWriting >>>> '%s' [%i]\n", pdata.filename, outSize);
    pdata.filesize = outSize;
    pdata.key = key;
    pdata.parameter = parameter;

    //pdata
    fwrite(&pdata,sizeof(pdata), 1, packedEXE);
    //compressed data
    fwrite(output, outSize, 1, packedEXE);

    fclose(inFile);
    fclose(packedEXE);

/********************************************************************
    7. Edit dos header to mark archive starting positing in exe.
*********************************************************************/
    setInsertPosition(dPath, stubSize);

    printf("File created: %s\n", dPath);
    return PESuccess;
}

/********************************************************************
   This function will edit the new EXE dos header to mark
   the starting position of the packed image EXE inside it by
   editing the reserved e_res2
*********************************************************************/
int setInsertPosition(char *filename, long pos){
    //open the file for reading and updating in binary
    FILE *packArchive = fopen(filename, "rb+");
    if (packArchive == NULL)
        return PEerrorCouldNotOpenArchive;

    //to capture the header
    IMAGE_DOS_HEADER idh;

    // read dos header and save it to idh
    fread((void *)&idh, sizeof(idh), 1, packArchive);

    //overwrite the e_res2 with pos(starting position of archive)
    *(long *)&idh.e_res2[0] = pos;

    //update header
    rewind(packArchive);
    fwrite((void *)&idh, sizeof(idh), 1, packArchive);
    fclose(packArchive);
    return PESuccess;
}

/********************************************************************
   This function will check if the input EXE file is a valid
   EXE file
*********************************************************************/
int validExeFile(const char *sPath){
    FILE *fp = fopen(sPath, "rb");
    UCHAR *content;

    fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    content = (UCHAR *) malloc (size*sizeof(UCHAR));
    fread(content, size, 1, fp);

    printf("Checking %s \n", sPath);

    fclose(fp);

    IMAGE_DOS_HEADER image_dos_header; //Line 2926
    IMAGE_NT_HEADERS image_nt_header; //Line 2981

    // Grab the DOS Headers
    memcpy(&image_dos_header,content,sizeof(image_dos_header));

    // Checking for dos signature
    if(image_dos_header.e_magic != IMAGE_DOS_SIGNATURE){
        printf("DOS Signature (MZ): INVALID\n\n");
        return PEerrorInputNotEXE;
    } printf("DOS signature (MZ): VALID\n");

    // Grab NT Headers
    memcpy(&image_nt_header,(LPVOID)((long int)content+image_dos_header.e_lfanew),sizeof(image_nt_header));

    // Checking for NT sigmature
    if(image_nt_header.Signature != IMAGE_NT_SIGNATURE){
        printf("PE Signature (PE00): INVALID\n\n\n");
        return PEerrorInputNotEXE;
    }else{
        printf("PE Signature (PE00): VALID\n\n\n");
        return 1;
    }
}
