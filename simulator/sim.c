#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// registers
unsigned char rA = 0;
unsigned char rB = 0;
unsigned char Ir = 0;
unsigned char Or = 0;
unsigned char pc = 0;
// flags
int haltFlag = 0;
int zeroFlag = 0;
int carryFlag = 0;

// declearation of functions

void print_help();
void process_arguments(int argc, char **argv, char **filename, int *maxSteps);
unsigned char *read_file(char *fileName, int *memSize);
void coreDump();
void memoryDump(unsigned char *memory, int memSize);
void simulateCPU(unsigned char *memory, int memSize, int maxSteps);
void AlU(int operation);

int main(int argc, char **argv)
{
    /*
    steps:
        1) get arguments
        2) read the file
        3) run simulation
    */

    char *fileName = NULL;
    int maxSteps = 3000;
    int memSize = 16;
    unsigned char *memory = malloc(memSize);

    // pocess_arguments(argc, argv, &fileName, &maxSteps);

    // // read the file
    // unsigned char *memory = read_file(fileName, &memSize);

    // return 0;

    // // run simulation
    // run_simulation(memory, memSize, maxSteps);

    simulateCPU(memory, memSize, maxSteps);

    // print out memory and core
    memoryDump(memory, memSize);
    coreDump();

    free(memory);
    return 0;
}

void simulateCPU(unsigned char *memory, int memSize, int maxSteps)
{

    int currentStep = 0;

    while ((currentStep < maxSteps) && !haltFlag)
    {
        /*
        process for cpu
        fetch
            get memory from pc
        decode
            get opcode and operands
        execute
            do the operation
        */

        Ir = memory[pc] >> 4;
        Or = memory[pc] & 0x0f;

        pc++;
        switch (Ir)
        {
        case 0x0: // NOP
            pc--;
            printf("PC: %02X ", pc);
            printf("NOP\n");
            pc++;
            break;
        case 0x1: // LDA
            rA = memory[Or];
            break;
        case 0x2: // STA
            memory[Or] = rA;
            break;
        case 0x3: // LDB
            rB = memory[Or];
            break;
        case 0x4: // STB
            memory[Or] = rB;
            break;
        case 0x5: // ADD
            AlU(0);
            break;
        case 0x6: // SUB
            AlU(1);
            break;
        case 0x7: // jmp
            pc = Or;
            break;
        case 0x8: // JNZ
            if (!zeroFlag)
            {
                pc = Or;
            }
            break;
        case 0x9: // jnc
            if (!carryFlag)
            {
                pc = Or;
            }
            break;
        case 0xe: // HLT
            haltFlag = 1;
            break;
        case 0xf: // out
            // prints contents of register A to the screen in decimal
            printf("OUT: %d\n", rA);
            break;
        default:
            printf("Invalid Instruction\n");
            break;
        }

        currentStep++;
    }
    printf("Simulation ended after %d steps\n", currentStep);
}

void AlU(int operation)
{
    /*
    operations
        0: add
        1: sub
    */
    int result = 0;

    if (operation == 0)
    {
        result = rA + rB;
    }
    else if (operation == 1)
    {
        result = rA - rB;
    }

    if ((result & 0xFF) == 0)
    {
        zeroFlag = 1;
    }
    else
    {
        zeroFlag = 0;
    }

    if (result > 255 || result < 0)
    {
        carryFlag = 1;
    }
    else
    {
        carryFlag = 0;
    }

    rB = result & 0xff;
}

void coreDump()
{
    // prints out all registers values in hex and flags
    printf("Core Dump:\n");
    printf("\tPC: %02X\n", pc);
    printf("\tRA: %02X\n", rA);
    printf("\tRB: %02X\n", rB);
    printf("\tIR: %02X\n", Ir);
    printf("\tOR: %02X\n", Or);
    printf("\tFlags:\n");
    printf("\t\tZero: %d\n", zeroFlag);
    printf("\t\tCarry: %d\n", carryFlag);
    printf("\t\tHalt: %d\n", haltFlag);
}

void memoryDump(unsigned char *memory, int memSize)
{
    // prints out all memory values in hex
    printf("Memory Dump:\n");
    for (size_t i = 0; i < (memSize / 4); i++)
    {
        printf("\t%02lX: %02X %02X %02X %02X\n", i * 4, memory[i * 4], memory[i * 4 + 1], memory[i * 4 + 2], memory[i * 4 + 3]);
    }
}

// void process_arguments(int argc, char **argv, char **fileName, int *maxSteps)
// {

//     // check all arguments
//     for (size_t i = 0; i < argc; i++)
//     {

//         if (argv[i][0] == '-')
//         {
//             // check for flags
//             if (strcmp(argv[i], "-h") == 0)
//             {
//                 // print help
//                 print_help();
//                 exit(0);
//             }
//             else if (strcmp(argv[i], "-s") == 0)
//             {
//                 // change max steps
//                 *maxSteps = (int)*argv[i + 1];
//                 exit(0);
//             }
//         }
//         else
//         {
//             if (strstr(argv[i], ".bin") != NULL)
//             {
//                 // get file name
//                 *fileName = argv[i];
//             }
//         }
//     }

//     if (*fileName == NULL)
//     {
//         printf("No file name provided\n");
//         exit(1);
//     }
// }

// void print_help()
// {
//     printf("Usage: ./sim [options] file.bin");
// }

// unsigned char *read_file(char *fileName, int *memSize)
// {
//     // open file
//     FILE *file = fopen(fileName, "rb");

//     // get file size
//     fseek(file, 0, SEEK_END);
//     long memSize = ftell(file);
//     rewind(file);

//     memSize = memSize < 16 ? 15 : memSize; // make sure memSize is at least 16

//     // allocate memory
//     unsigned char *memory = malloc(memSize);

//     // read file
//     fread(memory, 1, memSize, file);

//     // close file
//     fclose(file);

//     return memory;
// }