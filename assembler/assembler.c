/*
 *  Compiler for 8 bit computer
 *
 *  Program to assemble source code into machine code
 *  for the 8 bit computer.
 *
 *  Created by: Andres F. Bravo
 *  github: https://github.com/afbravo
 *
 *  Version 1.0
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// function prototypes
void processArgs(int argc, char **argv, char **inputFile, char **outputFile, int *memSize);
unsigned char *assemble(char *inputFile, int memSize);
void save_file(char *fileName, unsigned char *binary, int memSize);
unsigned char translate(const char *instruction);

void processArgs(int argc, char **argv, char **inputFile, char **outputFile, int *memSize)
{
    /*
        Function processes command line arguments. The following are
        accepted arguments:
            <input file> : specifies the input file name
            -o <output file> : specifies the output file name
            -h : prints the help message

        If no output file is specified, the output file name is "out.bin"
        If no input file is specified, the program exits with an error message

        Arguments:
            argc : number of command line arguments
            argv : pointer to array of command line arguments
            inputFile : pointer to the input file name
            outputFile : pointer to the output file name

        Returns:
            None

        TODO:
            - Add argument for specifying memory size
            - Modify help message
            - add .bin extension to output file if not specified
    */

    if (argc < 2) // user did not specify any file
    {
        printf("Error: No arguments specified\n");
        printf("Usage: comp [options] file\n"); // CHANGE COMP TO THE NAME OF THE EXECUTABLE
        exit(1);
    }

    for (int i = 1; i < argc; i++) // loop through all arguments
    {
        if (argv[i][0] == '-') // if argument is an option
        {
            switch (argv[i][1])
            {
            case 'h':                                         // display help message
                printf("HELP\nUsage: comp [options] file\n"); // ADD MORE HELP INFO
                exit(0);
            case 'o': // change output file name (add .bin extension)
                *outputFile = argv[i + 1];
                break;
            case 'm': // change memory size
                *memSize = atoi(argv[i + 1]);
                break;
            }
        }
        else // if argument is NOT an option, check for file name and type
        {
            // check if it ends in .asm
            if (strstr(argv[i], ".asm") != NULL)
            {
                *inputFile = argv[i];
            }
        }
    }

    if (*inputFile == NULL) // if no file name was specified exit
    {
        printf("Error: No input file specified or incorrect type\n");
        exit(1);
    }
}

unsigned char *assemble(char *fileName, int memSize)
{
    /*
        Function opens the input file containing assembly language for the 8-bit computer.
        It then reads each line and translates it into binary. The binary is then stored
        in an array of unsigned chars. The array is then returned.

        It is assumed that the input file has the following format:
            instruction <space> operand

        This is translated into a byte as follows:
            first nibble (4 bits) for the instruction
            second nibble (4 bits) for the operand

        Example:
            add 5 : 00100101

        Arguments:
            fileName : name of the input file
        Returns:
            binary : array of unsigned chars containing the binary code

        TODO:
            - Add support for comments
            - Add error checking for invalid instructions
            - Add error checking for invalid operands
    */
    FILE *file = fopen(fileName, "r");

    if (file == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    unsigned char *binary = (unsigned char *)malloc(memSize * sizeof(unsigned char));
    unsigned char index = 0;
    char instruction[4];
    int number;
    while (feof(file) == 0 && index < memSize)
    {
        fscanf(file, "%s %d", instruction, &number);
        number &= 0xFF;
        binary[index] = translate(instruction) | number;
        (index)++;
    }

    return binary;
}

unsigned char translate(const char *instruction)
{
    /*
        Function translates the instruction into binary. The binary return
        is shifted left by 4 bits to make room for the operand.

        The following instruction have the following binary values:
            NOP : 0000 | 0x00
            LDA : 0001 | 0x10
            STA : 0010 | 0x20
            LDB : 0011 | 0x30
            STB : 0100 | 0x40
            ADD : 0101 | 0x50
            SUB : 0110 | 0x60
            JMP : 0111 | 0x70
            JNZ : 1000 | 0x80
            JNC : 1001 | 0x90
            HLT : 1110 | 0xE0
            OUT : 1111 | 0xF0

        Arguments:
            instruction : instruction to be translated
        Returns:
            binary : binary byte representation of the instruction
    */
    if (strcmp(instruction, "lda") == 0)
    {
        return 1 << 4;
    }
    else if (strcmp(instruction, "sta") == 0)
    {
        return 2 << 4;
    }
    else if (strcmp(instruction, "ldb") == 0)
    {
        return 3 << 4;
    }
    else if (strcmp(instruction, "stb") == 0)
    {
        return 4 << 4;
    }
    else if (strcmp(instruction, "add") == 0)
    {
        return 5 << 4;
    }
    else if (strcmp(instruction, "sub") == 0)
    {
        return 6 << 4;
    }
    else if (strcmp(instruction, "jmp") == 0)
    {
        return 7 << 4;
    }
    else if (strcmp(instruction, "jnz") == 0)
    {
        return 8 << 4;
    }
    else if (strcmp(instruction, "jnc") == 0)
    {
        return 9 << 4;
    }
    else if (strcmp(instruction, "hlt") == 0)
    {
        return 14 << 4;
    }
    else if (strcmp(instruction, "out") == 0)
    {
        return 15 << 4;
    }
    else
    {
        return 0;
    }
}

void save_file(char *fileName, unsigned char *binary, int memSize)
{
    /*
        Function saves the binary array into a file.

        Arguments:
            fileName : name of the output file
            binary : array of unsigned chars containing the binary code
        Returns:
            None
    */
    FILE *file = fopen(fileName, "wb");

    if (file == NULL)
    {
        printf("Error opening file\n");
        exit(1);
    }

    fwrite(binary, sizeof(unsigned char), memSize, file);
    fclose(file);
}

// main function
int main(int argc, char **argv)
{

    // char *outputFileName = "out.bin";
    // char *inputFileName = NULL;
    // FILE *fptr;
    // int memSize = 16;

    // processArgs(argc, argv, &inputFileName, &outputFileName, &memSize);

    // // open file
    // // compile file to binary
    // unsigned char *binary = assemble(inputFileName, memSize);

    // // save binary file
    // save_file(outputFileName, binary, memSize);
    // return 0;

    int *a = (int *)malloc(5);
    printf("%d\n", a[0]);
    printf("%d\n", a[1]);
    printf("%d\n", a[2]);
    printf("%d\n", a[3]);
    return 0;
}