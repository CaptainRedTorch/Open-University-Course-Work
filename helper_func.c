#include <string.h>
#include <ctype.h>
#include <stdio.h>

#include "helper_func.h"
#include "constants.h"

/*Define a struct to hold Directive information*/
typedef struct Directive
{
    char *name;        /*Directive name*/
    int directiveCode; /*Number of operands*/
} Directive;

/*Define an array to hold all directive information*/
const Directive directiveTable[] = {
    {".data", 0},
    {".string", 1},
    {".entry", 2},
    {".extern", 3},
    {NULL, INVALID_DIRECTIVE} /*Terminating entry*/
};

unsigned int getDirectiveCode(char *name)
{
    int i;
    if (name == NULL)
    {
        return INVALID_DIRECTIVE;
    }
    for (i = 0; directiveTable[i].name != NULL; i++)
    {
        if (strcmp(directiveTable[i].name, name) == 0)
        {
            return directiveTable[i].directiveCode;
        }
    }
    return INVALID_DIRECTIVE;
}

/*Define a struct to hold operation information*/
typedef struct Operation
{
    char *name;          /*Operation name*/
    int num_operands;    /*Number of operands*/
    unsigned int opcode; /*Opcode value*/
} Operation;

/*Define an array to hold all operation information*/
const Operation opTable[] = {
    {"mov", 2, 0},
    {"cmp", 2, 1},
    {"add", 2, 2},
    {"sub", 2, 3},
    {"not", 1, 4},
    {"clr", 1, 5},
    {"lea", 2, 6},
    {"inc", 1, 7},
    {"dec", 1, 8},
    {"jmp", 1, 9},
    {"bne", 1, 10},
    {"red", 1, 11},
    {"prn", 1, 12},
    {"jsr", 1, 13},
    {"rts", 0, 14},
    {"stop", 0, 15},
    {NULL, 0, INVALID_OPCODE} /*Terminating entry*/
};

/*Function to retrieve the opcode for an operation*/
unsigned int getOpcode(char *opname)
{
    int i;
    if (opname == NULL)
    {
        return INVALID_OPCODE;
    }
    for (i = 0; opTable[i].name != NULL; i++)
    {
        if (strcmp(opTable[i].name, opname) == 0)
        {
            return opTable[i].opcode;
        }
    }
    return INVALID_OPCODE;
}

/*modified strtok function*/
char *strtok_single(char *str, const char *delim)
{
    static char *src = NULL;
    char *p, *ret = 0;
    if (str != NULL)
        src = str;
    if (src == NULL)
        return NULL;

    if ((p = strpbrk(src, delim)) != NULL)
    {
        *p = 0;
        ret = src;
        src = ++p;
    }
    else if (*src)
    {
        ret = src;
        src = NULL;
    }
    return ret;
}

/*function to clean whitespaces from string ends*/
void clean_whitespace(char **str_ptr)
{
    char *end;
    char *str = *str_ptr;
    if (str_ptr == NULL || *str_ptr == NULL)
        return;
    /*remove leading whitespace*/
    while (isspace((unsigned int)*str))
    {
        str++;
    }

    /*remove trailing whitespace*/
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned int)*end))
    {
        end--;
    }
    *(end + 1) = '\0';

    *str_ptr = str;
}

int is_instant(char *str)
{
    if (str[0] == '#' && is_number(str + 1))
        return true;
    return false;
}

int is_register(char *str)
{
    if (str[0] == 'r' && str[1] >= '0' && str[1] <= '7' && str[2] == '\0')
        return true;
    return false;
}

int is_number(char *str)
{
    int i, length = strlen(str);
    if (length < 1)
        return false;
    else if (str[0] == '-')
        i = 1;
    else if (str[0] == '+')
        i = 1;
    else
        i = 0;
    for (; i < length; i++)
    {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    sscanf(str, "%d", &i);
    if (i > MAX_INTEGER_12 || i < MIN_INTEGER_12)
        return false;
    return true;
}

int is_data_number(char *str)
{
    int i, length = strlen(str);
    if (length < 1)
        return false;
    else if (str[0] == '-')
        i = 1;
    else if (str[0] == '+')
        i = 1;
    else
        i = 0;
    for (; i < length; i++)
    {
        if (str[i] < '0' || str[i] > '9')
            return false;
    }
    sscanf(str, "%d", &i);
    if (i > MAX_INTEGER_14 || i < MIN_INTEGER_14)
        return false;
    return true;
}

int register_number(char *str)
{
    return str[1] - '0';
}

int is_string(char *str)
{
    int i, length = strlen(str);
    if (str[0] == '"')
    {
        for (i = 1; i < length; i++)
        {
            if (isprint(str[i]))
            {
                if (str[i] == '"')
                {
                    if (i == length - 1)
                        return true;
                    else
                        return false;
                }
            }
            else
                return false;
        }
    }
    return false;
}