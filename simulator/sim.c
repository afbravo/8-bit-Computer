#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// declearation of functions

void print_help();
void process_arguments(int argc, char **argv, char **filename, int *maxSteps);
unsigned char *read_file(char *fileName);

int main(int argc, char **argv)
{
    /*
    steps:
        1) get arguments
        2) read the file
        3) run simulation
    */

    char *fileName = NULL;
    int maxSteps = 500;

    process_arguments(argc, argv, &fileName, &maxSteps);

    // read the file
    unsigned char *memory = read_file(fileName);

    int size = sizeof(memory) / sizeof(memory[0]);

    printf("size: %d\n", size);

    for (int i = 0; i < size; i++)
    {
        printf("%d ", memory[i]);
    }
    printf("\n");

    return 0;

    // run simulation
    // run_simulation(memory, maxSteps);
}

void process_arguments(int argc, char **argv, char **fileName, int *maxSteps)
{

    // check all arguments
    for (size_t i = 0; i < argc; i++)
    {

        if (argv[i][0] == '-')
        {
            // check for flags
            if (strcmp(argv[i], "-h") == 0)
            {
                // print help
                print_help();
                exit(0);
            }
            else if (strcmp(argv[i], "-s") == 0)
            {
                // change max steps
                *maxSteps = (int)*argv[i + 1];
                exit(0);
            }
        }
        else
        {
            if (strstr(argv[i], ".bin") != NULL)
            {
                // get file name
                *fileName = argv[i];
            }
        }
    }

    if (*fileName == NULL)
    {
        printf("No file name provided");
        exit(1);
    }
}

void print_help()
{
    printf("Usage: ./sim [options] file.bin");
}

unsigned char *read_file(char *fileName)
{
    // open file
    FILE *file = fopen(fileName, "rb");

    // get file size
    fseek(file, 0, SEEK_END);
    long fileSize = ftell(file);
    rewind(file);

    printf("file size: %ld\n", fileSize);

    // allocate memory
    unsigned char *memory = malloc(fileSize);

    // read file
    fread(memory, 1, fileSize, file);

    // close file
    fclose(file);

    int size = sizeof(memory) / sizeof(memory[0]);

    printf("size: %d\n", size);

    for (int i = 0; i < fileSize; i++)
    {
        printf("%d ", memory[i]);
    }

    printf("\n");

    return memory;
}
