#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "preAsmblr.h"
#include "firstPass.h"
#include "secondPass.h"
#include "constants.h"
#include "globals.h"

void intro();

int main(int argc, char *argv[])
{
    char *filename;
    int i; /* loop counter */
    int first; /*first pass statues*/
    int second; /*second pass statues*/
    if (argc < 2)
    {
        printf("\nError: no files were provided as operands\n");
        return 0;
    }

    intro();

    /*for every operand in the command try to assmble as file*/
    for (i = 1; i < argc; i++)
    {
        /*reset globlas*/
        symbol_table_head = NULL;
        deleteBinaryTable();
        PC = 100;
        IC = 0;
        DC = 0;
        line_number = 0;
        error_flag = false;

        /*start to procces file*/
        filename = fileHandler(argv[i]);
        if (filename == NULL)
        {
            continue;;
        }
        printf("\nFile %s:", argv[i]);
        first = firstPass(argv[i]);
        second = secondPass(argv[i]);
        if (first == true || second == true)
        {
            printf("\nErrors were found: files were not created for \"%s\"\n\n", filename);
            continue;;
        }
        printf("\nFiles were created for \"%s\"\n\n", filename);
    }
    return 0;
}

void intro()
{
    printf("\033[4;36m");
    printf("\n\n\tMMN14 - ASSEMBLER\n\n");
    printf("\033[0m");
    printf("\033[2;37m");
    printf("Author: [your name]\n");
    printf("\nLimitations: \n");
    printf("\t- The files given in the command line should be of type .as\n");
    printf("\t- Lines in the .as file should not exceed 80 characters\n");
    printf("\t- Labels should not exceed 30 characters\n");
    printf("\t- The machine as 256 cells of memory\n");
    printf("\t- The machine has 8 registers\n");
    printf("\t- Integer numbers for .data should be between 8191 to -8192\n");
    printf("\t- Integer numbers for instant should be between 2047 to -2048\n");
    printf("\t- String should consist of printable ascii characters\n");
    printf("\n\n");
    printf("\033[0m"); 


}
