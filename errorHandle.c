#include <stdio.h>
#include <string.h>

#include "errorHandle.h"

void errorHandle(int error, int line_number, char *line, char *token)
{
    printf("\033[1;31m");
    printf("\n%d: ", line_number);
    printf("\033[0;37m");
    if (line != NULL)
    {
        /*prints with \n after the original line regardless*/
        if (line[strlen(line) - 1] == '\n')
        {
            printf("%s", line);
        }
        else
            printf("%s\n", line);
    }
    printf("\033[1;31m");
    printf("Error: ");
    printf("\033[0;37m");
    switch (error)
    {
    case LABEL_ENTRY_EXTERN:
    {
        printf("label can't be both entry and extern ");
        break;
    }
    case LABEL_DEFINED_ENTRY:
    {
        printf("label can't be defined twice as entry ");
        break;
    }
    case LABEL_NOT_DEFINED:
    {
        printf("label not defined ");
        break;
    }
    case INVALID_LABEL_NAME:
    {
        printf("invalid label name ");
        break;
    }
    case INVALID_LABEL_LENGTH:
    {
        printf("invalid label length ");
        break;
    }
    case INVALID_LABEL_NULL:
    {
        printf("invalid label is empty ");
        break;
    }
    case INVALID_LABEL_RESERVED:
    {
        printf("invalid label is a reserved word ");
        break;
    }
    case INVALID_LABEL_FIRST:
    {
        printf("invalid label first character ");
        break;
    }

    case LABEL_NOT_FOUND:
    {
        printf("label not found ");
        break;
    }
    case LABEL_ALREADY_EXISTS:
    {
        printf("label already exists ");
        break;
    }
    case INVALID_STRING:
    {
        printf("invalid string ");
        break;
    }
    case INVALID_STRING_NULL:
    {
        printf("invalid missing string ");
        break;
    }
    case INVALID_DATA:
    {
        printf("invalid data type ");
        break;
    }
    case INVALID_DATA_SYNTAX:
    {
        printf("extraneous comma's ");
        break;
    }
    case INVALID_COMMAND:
    {
        printf("invalid unknown command ");
        break;
    }
    case INVALID_MISSING_OPERAND:
    {
        printf("missing operand ");
        break;
    }
    case INVALID_EXTRA_OPERAND:
    {
        printf("extra operand ");
        break;
    }
    case INVALID_OPERAND_DEST:
    {
        printf("invalid operand destination ");
        break;
    }
    case INVALID_OPERAND_SRC:
    {
        printf("invalid operand source ");
        break;
    }
    case INVALID_MACRO_NAME:
    {
        printf("invalid macro name ");
        break;
    }
    case INVALID_MACRO_NULL:
    {
        printf("invalid macro is empty ");
        break;
    }
    case INVALID_MACRO_EXIST:
    {
        printf("invalid macro already exists ");
        break;
    }
    case INVALID_MACRO_SYNATX:
    {
        printf("invalid macro syntax ");
        break;
    }
    case INVALID_LINE_LENGTH:
    {
        printf("invalid line length ");
        break;
    }
    case INVALID_RELATIVE_LABEL:
    {
        printf("invalid label name in addressing ");
        break;
    }
    case INVALID_RELATIVE:
    {
        printf("invalid relative addressing ");
        break;
    }
    case INVALID_OPERAND:
    {
        printf("invalid operand ");
        break;
    }
    }
    if (token != NULL)
    {
        printf("\033[0m");
        printf("\033[0;35m");
        printf("%s", token);
        printf("\033[0;37m");
    }
    printf("\n");
    printf("\033[0m");
}

/*
    Error guide:
    LABEL_ENTRY_EXTERN, - label can't be both entry and extern
    LABEL_DEFINED_ENTRY, - label can't be defined twice as entry
    LABEL_NOT_FOUND, - label not found
    LABEL_ALREADY_EXISTS, - label already exists
    INVALID_LABEL_NAME, - invalid label name
    INVALID_LABEL_LENGTH, - invalid label length
    INVALID_LABEL_NULL, - invalid label is null or empty
    INVALID_LABEL_RESERVED, - invalid label is a reserved word
    INVALID_LABEL_FIRST, - invalid first character in label
    INVALID_STRING, - invalid string
    INVALID_STRING_NULL, - missing string
    INVALID_DATA, - invalid in .data type
    INVALID_DATA_SYNTAX, - extraneous comma's in .data
    INVALID_COMMAND, - invalid unknown command
    INVALID_MISSING_OPERAND, - missing operand
    INVALID_EXTRA_OPERAND, - extra operand
    INVALID_OPERAND_DEST, - invalid operand destination
    INVALID_OPERAND_SRC, - invalid operand source
    INVALID_MACRO_NAME, - invalid macro name
    INVALID_MACRO_NULL, - macro is empty
    INVALID_MACRO_EXIST, - macro already exists
    INVALID_MACRO_SYNATX, - invalid macro syntax
    INVALID_LINE_LENGTH - invalid line length
    INVALID_RELATIVE_LABEL - invalid name of label in relative addressing
    INVALID_RELATIVE - invalid relative addressing
    INVALID_OPERAND - general invalid operand
*/
