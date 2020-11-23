# hXOR Packer
hXOR Packer is a PE (Portable Executable) packer with Huffman Compression and Xor encryption.

The unpacker will decompress and decrypt the packed PE and execute it directly from memory 
without needing any hard disk space to execute.  

This is my old school final year polytechnic project 2012 back then.  

For learning purposes, i will just copy paste the useful info from my rusty FYP report in the introduction onwards.

## How it is done?
1. Dynamic Process Forking Of Portable Executable
2. Huffman entropy encoding algorithm
3. Simple XOR encryption

## Usage 
```
For Packing:  
(S) -> EXE File (Absolute Path)  
(D) -> Destination Output (Absolute Path)  
(P) -> Parameters (Optional)  
(K) -> Xor Encryption Key in numbers (Optional)  
 
Avaliable Parameters (Optional):   
-c              Compression   
-e              Encryption   
-ce             Compression & Encryption 

Examples:
- packer.exe (S) (D) (P) (K)   
- packer.exe C:\in.exe C:\folder\out.exe   
- packer.exe C:\in.exe C:\folder\out.exe -ce 56213 
```

# Tools used
## Code::Blocks
Code::Blocks is a free and open source cross-platform integrated development environment. It
provides a source code editor, build automation tools, debugger and more. Code::Blocks uses lesser
memory than the Microsoft Visual Studio. Code::Blocks also supports multiple compilers such
as, MinGW or GCC, Digital Mars, Microsoft Visual C++, Borland C++, Watcom, LCC and the Intel C++
compiler.
## MinGW
MinGW (28), which stands for Minimalist GNU for Windows, is a port of the GCC, GNU
Compiler Collection. This is my primary compiler when I am compiling any C/C++
applications.

# Knowledge and skills required
- Portable Executable file format
- Huffman entropy encoding algorithm
- Dynamic Process Forking Of Portable Executable
- XOR Encryption

# Introduction
The aim of this project is to create a Portable Executable Packers written in C++, which supports
compression using Huffman algorithm, XOR encryption and executing an executable file from
memory. I use Code::Blocks IDE and GCC compiler to make this project successful.

In English language, packing means to prepare one or more items for proper storage and be ready
for transportation. In this case, packing means to prepare one or more programs or a set of codes
and compress it into one executable self-extracting archive, which is then ready to execute in any
machine with an operating system capable reading the format of the packer, which is the Portable
Executable Format.  

The format of a Portable Executable (PE) is the same for every executable file except for .NET PE. The
operating system will read each every bit in the header as it contains the information on how the
operating system should run the executable file.  

# Scope
To create a PE (Portable Executable) Packer that allows users to crypt the source code of their
program and compress the encrypted source code so that they will be not be detected and deleted
by the antivirus.

# Purpose
The purpose of this project is to successfully infect a person operating system with the PE Packer.
Inside the PE Packer, it will contain a encrypted source code of a virus and it will unpack, decrypt and
compile itself during runtime. 
## How does the hXOR Packer Works?
By packing, it means to pack a executable file and either compress or encrypt it, as such, it nearly
impossible for a user to notice a difference between a Packed Executable File and the
original/unpacked Executable File without having the knowledge of reading the Packed Executable
file in hexadecimal form, from a hex editor.

The Portable Executable Packer that I’m writing will called hXOR.exe, which “h” simply stands for the
Huffman algorithm and the XOR stands for “XOR” encryption that being used.
Firstly, hXOR packer will validate if the input file is a valid executable file. By doing this validation, it
will helps hXOR Un-Packer as it will assume that the executable file that it suppose to execute from
memory is a valid executable file

Secondly, it will check for any parameter that is being entered during execution. It can either;
1. Packs the Executable file without any compression nor encryption,
2. Packs the Executable file with only compression,
3. Packs the Executable file with encryption with key provided by the user,
4. Packs the Executable file with encryption without key provided by the user,
5. Packs the Executable file with both compression and encryption with the key provided by
the user,
6. Packs the Executable file with both compression and encryption without the key provided by
the user.

Lastly, the packer will pack the executable file along the Un-Packer to create an output packed
executable file. By adding the Un-Packer application at the very beginning of the encrypted and
compressed executable file, the Un-Packer will decompress, decrypt and finally, execute the packed
PE, making it impossible for normal user to notice that he/she has executed a packed PE. 

### Functional Requirement
- Packs input file
- - User enter the input file directory
- - The software will inform the user the output with only the output file directory.
- Packs with Compression
- - User enters input file directory with ‘-c’ as the parameter upon execution.
- - The software compresses the input file.
- - The software will inform the user the output with the output file size and output file
directory on the command line.
- Packs with Encryption without key provided
- - User enters the input file directory with ‘-e’ as the parameter upon execution.
- - The software will generate the key.
- - The software encrypts the input file with the key generated.
- - The software will inform the user the output with only the output file directory.
- Packs with Encryption with key provided
- - User enters the input file directory with ‘-e x’ as parameter where ‘x’ is the key and is in
integer upon execution.
- - The software will encrypt the input file with the provided key.
- - The software will inform the user the output with only the output file directory.
- Packs with Compression and Encryption without the key provided
- - User enters the input file directory with ‘-ce’ as the parameter upon execution.
- - The software will generate the key.
- - The software will encrypt the input file with the key generated.
- - The software will then compresses the encrypted input file
- - The software will inform the user the output with the output file size and output file
directory on the command line.
- Packs with Compression and Encryption with the key provided
- - User enters the input file directory with ‘-ce x’ as the parameter where ‘x’ is the key and
is in integer upon execution.
- - The software will encrypt the input file with the provided key.
- - The software will then compresses the encrypted input file
- - The software will inform the user the output with the output file size and output file
directory on the command line.
### Non-Functional Requirement
- Able to validate input file
- - The software must have the ability to check the input file to make sure it is an
executable file.
- Able to accepts and validate parameters
- - The software must have the ability to accept any parameter and validate them.
- Command-line Interface
- - The software must have a command-line interface.

## How does the hXOR Un-Packer Works?
The Un-Packer is the only program that will not require a single input from the user during runtime.
It is designed to have no graphical user interface.

Firstly, the Un-Packer will look for the offset to the Packed Executable file in its own Image DOS
Header. It will then create a pointer pointing to the starting position of the Packed Executable File.
Secondly, the Un-Packer will begin to understand how the Packed Executable file was being packed
by the Packer in order to unpack it.

Lastly, without saving the Unpacked Executable File into the system hard disk, the Un-Packer will
begin to create a child process and execute the Unpacked Executable File from the memory. It is the
nearly impossible for a normal user to notice that the Executable File he/she just executed is actually
a Packed Executable File and all this 3 steps have just happened quickly during execution.
### Functional Requirement
None
### Non-Functional Requirement
- Able to locate the Packed File
- - The program must be able to locate the Packed File inside of itself.
- Able to understand the Packed File
-  The program must be able to understand how the file was packed by the Packer, hXOR.
- Invisible to the user
- - The program must have no graphical interface and it will not open a command prompt
window too.
- Output visible in command line
- - The program will be able to show output if user execute it from the command line
instead of double clicking in the explorer.exe.
- Execute from memory
- - The program will have to execute the unpacked Executable File from the memory.
- Able to scan running processes and detect environment
- - The program will be able to detect if there is any defensive software that is interfering
with the execution of itself, such as runtime scanning and isolated environment such as
Sandboxie.

# Binders vs Packers
## Binders
A binder can accept and packs multiple file such as, executable files, pictures, music and more. While
there are other alternatives such as, RAR, ZIP and 7z, a binder is a self-extracting archives which able
to unpack itself and saves all the packed files to the system hard disk. This is useful for the receiver
as he/she does not have to install an application to unpacks what he/she just received.
If the user wants to execute or open a file inside a binder, it will create a copy of itself in the hard
disk and execute from there. If a virus is using this method, the virus can be easily detected by most
Antiviruses. 

![Binders Demo](https://cdn.discordapp.com/attachments/240938506103422976/779000931571466240/unknown.png)

## Packers
While a binder can packs multiple file, a packer can only accepts one. It is commonly used to packs,
compress and encrypt an executable file, which we will call it “Packed Executable File”.
Think of it as an executable file, inside another executable file, which also can be inside another
executable file.

Packer is well known to be used by malware authors and hackers because by using this method, it
will be harder for the antivirus to detect their software.

However, not all Packed Executable File is created for bad purposes. There are Software Companies
that uses Packer to distribute their software to their customers. The main reason is to be able to
reduce its application size during distributing.

For example, if the application installer is more than 1 GB, the company will try to reduce it size to
700 MB so that it can save money by distributing it into a single CD-ROM rather than two CD-ROM or
one DVD-ROM. Another reason is the bandwidth if the company decides to distribute it through the
Internet. 

As there are more negative purpose of Packer than the positive ones, Antivirus Companies often
label Packer that is unreadable by the antivirus as malware, suspicious or infected file, that which
will be mark as false positive once they review it. 

![Packer Demo](https://cdn.discordapp.com/attachments/240938506103422976/779001081741049856/unknown.png)


# Portable Executable
Portable Executable, also called PE, is a file format, usually in .exe, .dll and .sys file extensions. It
was first introduced by Microsoft which first used in Windows NT version 3.1 in the year 1993. It can
only be execute in Windows operating system or Linux/Unix-like operating system using WINE (Wine
Is Not an Emulator) compatibility layer.

### Portable Executable File Structure
PE file format is a modified version of the Unix COFF (Common Object File Format), a specification
format for UNIX system to execute, object codes and shared library computer files. To make it
compatible with previous version of Windows, the PE file format will retains the old popular MZ
header from MS-DOS.

The PE file format is organized in a linear stream of data. I will focus on the field in the headers that
is relevant and important for the user and the system. First, it begins with:

![PE Header](https://cdn.discordapp.com/attachments/240938506103422976/779010925805174784/unknown.png)

### Image DOS header
All EXE files will have a MZ header. To verify if a file can be executed in DOS, a signature of “MZ” at
the very first 2 bytes will be present which is declared e_magic below. Another very important field
in the DOS header is e_lfanew, an offset to the PE header.

![Image DOS header](https://cdn.discordapp.com/attachments/240938506103422976/779012034817687582/unknown.png)

### DOS stub program
The DOS stub program is useful only for DOS system. It is present in every Win32 Executable file. The
purpose of this stub is to ensure that the Win32 program will not crash when it is run on a DOS
system. It will an output an error message such as, “This program cannot be run in DOS mode.” And
exit.

### PE File Signature
By using the e_lfanew located at the MS DOS header, you can get the offset to this PE header, which
is also right after the DOS-Stub program. To ensure that this is a PE file, a signature for this, “PE00”,
will be present in the signature field.

![PE File Signature](https://cdn.discordapp.com/attachments/240938506103422976/779015199490834523/unknown.png)

### Image File Header
Right after the PE header is the PE File Header. The important information that most users are
interested to know in this header is,
1. It will inform the operating system what “Machine” it is intended to run,
2. The number of sections in the section table.
3. Size of optional header, mainly used to know the correctness of the PE file structure

![Image File Header](https://cdn.discordapp.com/attachments/240938506103422976/779018406925565982/unknown.png)

### Image File Optional Header
Following the File Header is the PE File Optional Header. Despite the name being optional, it will
always be present in every PE file and it contains very important information on how to treat the PEfile exactly.
The important information that most users are interested to know are,
1. The SizeOfCode, intended to be the size of the executable code.
2. SizeOfInitializedData, the size of initialized data.
3. SizeOfUninitializedData, the size of uninitialized data.
4. AddressIfEntryPoint, offset to the code’s entry point. This is used by the user if he loads the
image by hand, for example, from memory. This address will start the process after the user
has finish with all the fixups and the relocations.
5. ImageBase, this is the address that the file has been relocated by the linker.
6. SectionAlignment
7. SizeOfImage, the sum of all the headers and section length if aligned to the
“SectionAlignment”
8. SizeOfHeaders, the total length of all the headers and including the data directories and
section headers.
9. NumberOfRvaAndSizes, the number of valid entries in the directories.
10. And the Arrays of DataDirectory.

![Image File Optional Header](https://cdn.discordapp.com/attachments/240938506103422976/779020601007407134/unknown.png)

### Section Tables
The Section Tables is located right after the optional header in the PE file format. This section Tables
contains an array of structure and the number of array is determined by NumberOfSections field in
the File Header above it.
Each Section has its own,
1. Name
2. Permission
3. Size
The operating system will,
1. Read the NumberOfSections in the PE File Header so that it will know how many section are
there in the file
2. Use the SizeOfHeader as the file offset of the section table and moves the file pointer to that
offset
3. It will obtain the Pointer ToRawData in this header mover the file pointer to that offset.
4. Then, it will read the value in the SizeOfRawData field so that the system will know how man
bytes should he map to the memory and mark the attributes of the mapped memory using
the value in the Characteristics field in this header.

![Section Tables Header](https://cdn.discordapp.com/attachments/240938506103422976/779023336419229716/unknown.png)

## Section Bodies
Section bodies are the actual PE file data. It will depend on the section headers on how to use them.
### .NET PE File Structure
Microsoft .NET Framework has extended the PE file format with features that supports the Common
Language Runtime. There are two new sections included, CLR Header and CLR Metadata.
### CLR Header
In order to get to the CLR Header, we have first to get to the 15th entry IMAGE_DATA_DIRECTORY in
the Optional Header. It contains the offset to the CLR Header.

![Section Bodies](https://cdn.discordapp.com/attachments/240938506103422976/779023649378140190/unknown.png)
### CLR Metadata
The CLR Metadata is directly after the CLR Header. It contains a signature and version information.

![Section Bodies](https://cdn.discordapp.com/attachments/240938506103422976/779023860414021653/unknown.png)

## Huffman Entropy Encoding Algorithm
The Huffman coding uses a statistical technique. By using only C functions like, memset, memmove,
qsort, malloc and memcpy, it can reduce the amount of bits used to represent a string of characters
without using any external library such as STL or BoxedApp and more. It did not use a dictionary for
reference as Huffman uses a tree.
### Header
The header will be created by the compression program and will be used by the decompression
program. It stores initial information for the decompression program to reconstruct the Huffman
tree that was used by the compression program to compress the original file. It usually be located at
the beginning of the compressed file.
### Compressing
Firstly, a tree needs to be build in order to compress data using Huffman Algorithm
#### Building a Huffman tree
The Huffman tree contains nodes that representing the character, frequency, code and code length.
Here is one example of a Huffman tree.
An example:

![Huffman tree](https://cdn.discordapp.com/attachments/240938506103422976/779024203964743680/unknown.png)

The Huffman tree is constructed by following this step:
1. Create a parentless node which can also known as child node, is being creating first. It will
contain the character and the frequency of occurrence.
2. Select any two nodes with the lowest frequency.
The only nodes containing character H, U, M, A, N are valid.
Character F is not valid as it is currently has the highest frequency, 2.
3. Create a new node which will be the parent of the two previously selected nodes.
The parent node will only contain the sum of the frequency of its child.
4. Repeat the step 2 and 3 until you reach only one parentless node left, which will be the root.
Hence, the string “HUFFMAN” will be written as 000001101001011111 as the output of the
compression.

### Decompressing
Decompressing is very straight forward. The program will look for the tree in the compressed file
header. It will then reconstruct the tree and use that information to get the original output or state
of the compressed file.

The decompression program has to return the data to its original
state, from binary 000001101001011111 to the string “HUFFMAN”.
1. It will reconstruct the tree by reading the file header. The header which, the compression
program stores the initial information of the Huffman tree.
2. It will then use the code and code length to find the parentless node which contains the
original character.


## Dynamic Process Forking Of Portable Executable
When Windows open up any file, it will make a copy of it into the memory. The same goes for
executable file.

When the file is in the memory, it will be called a module. The beginning of the file address is can be
called HMODULE. By using the GetModuleFileName() API, you will be able to get the address to file
in the memory, giving you the ability to locate the file and able to read or write the memory space.
The data structure of the PE file on disk will be the same if it is loaded into the memory.

Therefore, they are equal and loading a PE into memory is just mapping some ranges of the
executable into the address space. What this also means, the data structure of the MZ DOS header
on the disk will be equal to that in the memory. Windows usually decide which essential parts of the
PE file should be mapped.

The un-packer will create a new child process and replace its content with the unpacked executable
file content. This allows the unpacked executable file load directly from memory.

![Dynamic Process Forking Of Portable Executable](https://cdn.discordapp.com/attachments/240938506103422976/779024613426069514/unknown.png)

## XOR Encryption
Exclusive OR (XOR) encryption is and encryption that is almost unbreakable by brute force methods.
### How it works?
It is very simple encryption to use and it strong brute force. The encryption uses bitwise XOR
operation on the original data and a key. It will produce an encrypted data.

The encrypted data can only be decrypted by using the bitwise XOR operation on the encrypted data
and the same key.

In XOR Encryption, I can reuse the same code for encrypting and decrypting. 

![XOR Encryption](https://cdn.discordapp.com/attachments/240938506103422976/779024846007828500/unknown.png)
