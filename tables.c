
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "tables.h"
#include "helper_func.h"
#include "constants.h"
#include "globals.h"
#include "errorHandle.h"

extern int error_flag;
extern int line_number;
int symbol_table_size = 0; /*used for symbol ID*/
struct symbol_t *symbol_table_head = NULL;
binary_t binary_table[MAX_FILE_LENGTH];

/*symbol table functions*/
int is_valid_label(char *label)
{
    size_t i;
    /*Check if the label is empty or NULL*/
    if (label == NULL || (int)label[0] == '\0')
    {
        return INVALID_LABEL_NULL;
    }
    /*Check if the first character is valid*/
    if (!isalpha((int)label[0]))
    {
        return INVALID_LABEL_FIRST;
    }
    /*Check if the rest of the characters are valid*/
    for (i = 1; label[i] != '\0'; i++)
    {
        if (!isalnum((int)label[i]))
        {
            return INVALID_LABEL_NAME;
        }
    }
    /*Check if the label is too long*/
    if (strlen(label) >= MAX_LABEL_LENGTH)
    {
        return INVALID_LABEL_LENGTH;
    }
    /*Check if the label is a reserved word*/
    if (getOpcode(label) != INVALID_OPCODE || getDirectiveCode(label) != INVALID_DIRECTIVE || is_register(label) == true)
    {
        return INVALID_LABEL_RESERVED;
    }

    return true;
}

symbol_t *get_symbol(char *name)
{
    /* Find the symbol in the symbol table return pointer to symbol*/
    symbol_t *current_symbol = symbol_table_head;

    while (current_symbol != NULL)
    {
        if (strcmp(current_symbol->name, name) == 0)
        {
            return current_symbol;
        }

        current_symbol = current_symbol->next;
    }

    return NULL;
}

unsigned int get_symbol_ID(char *name)
{
    /* Find the symbol in the symbol table return symbol ID*/
    symbol_t *current_symbol = get_symbol(name);
    if (current_symbol == NULL)
    {
        return -1;
    }
    return current_symbol->symbol_ID;
}

void add_symbol(char *name, int value, int is_data, int is_relocatable, int is_entry, int is_defined, int is_used)
{
    symbol_t *symbol = (symbol_t *)malloc(sizeof(symbol_t));

    symbol_t *exists = get_symbol(name);
    if (exists == NULL)
    {
        /*Add the symbol to the symbol table*/
        strncpy(symbol->name, name, MAX_LABEL_LENGTH);
        symbol->name[MAX_LABEL_LENGTH] = '\0';
        symbol->value = value;
        symbol->is_data = is_data;
        symbol->is_relocatable = is_relocatable;
        symbol->is_entry = is_entry;
        symbol->is_used = is_used;
        symbol->is_defined = is_defined;
        symbol->line_number = line_number;
        symbol->symbol_ID = symbol_table_size;
        /*link new symbol to list*/
        symbol_table_size++;
        symbol->next = symbol_table_head;
        symbol_table_head = symbol;

        return;
    }

    /*If symbol already exists checks*/

    if (exists->is_relocatable == false && is_entry == true)
    {
        errorHandle(LABEL_ENTRY_EXTERN, line_number, NULL, name); /*symbol cant be both entry and extern*/
        error_flag = true;
    }
    else if (exists->is_entry == true && is_entry == true)
    {
        errorHandle(LABEL_DEFINED_ENTRY, line_number, NULL, name); /*symbol cant be defined as entry twice*/
        error_flag = true;
    }
    else if (exists->is_entry == false && is_entry == true) /*symbols that exists being defined as entry*/
    {
        exists->is_entry = is_entry;
    }

    if (exists->is_defined == true && is_defined == true)
    {
        errorHandle(LABEL_ALREADY_EXISTS, line_number, NULL, name); /*label cant be defined twice*/
        error_flag = true;
    }
    else if (exists->is_defined == false && is_defined == true)
    {
        /*label was used in the past but new it is being defined*/
        exists->value = value;
        exists->is_data = is_data;
        exists->is_relocatable = is_relocatable;
        exists->is_defined = is_defined;
    }
    else if (exists->is_defined == true && is_defined == false)
    {
        /*label defined already but now its being used*/
        exists->is_used = is_used;
    }

    free(symbol);
    return;
}

/*deletes the symbol table*/
void deletSymbolTable()
{
    symbol_t *current_symbol = symbol_table_head;
    symbol_t *next_symbol = NULL;

    while (current_symbol != NULL)
    {
        next_symbol = current_symbol->next;
        free(current_symbol);
        current_symbol = next_symbol;
    }
}

/*Print the symbol table for debug*/
void printTable()
{
    symbol_t *current_symbol = symbol_table_head;
    printf("\n\nSymbol Table:\n");
    while (current_symbol != NULL)
    {
        printf("\n%s: value=%d|is_data=%d|is_relocatable=%c|is_entry=%c|is_used=%c|is_defined=%c|\n",
               current_symbol->name, current_symbol->value, current_symbol->is_data,
               (current_symbol->is_relocatable) ? 'R' : 'E', current_symbol->is_entry ? 'E' : ' ',
               current_symbol->is_used ? 'E' : ' ', current_symbol->is_defined ? 'E' : ' ');
        current_symbol = current_symbol->next;
    }
}

void add_Binary_register(int srcValue, int destValue, int PC)
{
    /*Adds register to the binary table*/
    binary_t binary;
    if (srcValue == -1)
    {
        /*is destination register*/
        binary.binary_code = (destValue << 2);
    }
    else if (destValue == -1)
    {
        /*is source register*/
        binary.binary_code = (srcValue << 8);
    }
    else
    {
        /*two register combined*/
        binary.binary_code = (srcValue << 8) | (destValue << 2);
    }
    binary.is_symbol = false;
    binary.is_data = false;
    binary_table[PC - 100] = binary;
}

/*binary table functions*/
void add_Binary_var(int value, int type, int PC)
{
    binary_t binary;
    /*Adds variables to the binary table*/
    if (type == BINARY_INSTANT)
    {
        binary.binary_code = (value << 2);
        binary.is_symbol = false;
        binary.is_data = false;
    }
    else if (type == BINARY_DATA)
    {
        binary.binary_code = (value << 0);
        binary.is_symbol = false;
        binary.is_data = true;
    }
    else if (type == BINARY_STRING)
    {
        binary.binary_code = (value << 0);
        binary.is_symbol = false;
        binary.is_data = true;
    }
    else if (type == BINARY_SYMBOL)
    {
        /*there is no possible way for the file to have more than 16383(14 bit unsigned max) symbols under the guidelines spespicications
         *which is why we can give each symbol an integer based code to be used in the binary table*/
        /*value = symbols ID value*/
        binary.binary_code = value;
        binary.is_symbol = true;
        binary.is_data = false;
        /*this will later be used in the second pass to convert to binary*/
    }
    else
    {
        return;
    }
    binary_table[PC - 100] = binary;
    return;
}

void add_Binary(int ARE, int dest, int src, int opCode, int p2, int p1, int PC)
{
    binary_t binary;

    /*Add the instruction word to the binary table*/
    binary.is_symbol = false;
    binary.is_data = false;

    binary.binary_code = (p1 << 12) | (p2 << 10) | (opCode << 6) | (src << 4) | (dest << 2) | ARE;
    binary_table[PC - 100] = binary;
    return;
}

/*resets the binary table*/
void deleteBinaryTable()
{
    int i;
    for (i = 0; i <= PC-100; i++)
    {
        binary_table[i].binary_code = 0;
        binary_table[i].is_symbol = false;
    }
}

/*Print the binary table for debug*/
void printBinary_t(int PC)
{
    int i;
    int j;
    printf("\nPrinting binary_t:");
    for (i = 0; i < PC - 100; i++)
    {
        printf("\n%d: ", i);
        for (j = 13; j >= 0; j--)
        {
            if (binary_table[i].binary_code & (1 << j))
            {
                printf("1");
            }
            else
            {
                printf("0");
            }
        }
    }
}