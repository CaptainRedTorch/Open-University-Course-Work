#ifndef tables_h
#define tables_h

#include <stdlib.h>

#include "constants.h"
#include "helper_func.h"

/*symbol table*/
typedef struct symbol_t
{
    char name[MAX_LABEL_LENGTH];
    unsigned int value;
    unsigned int is_data : 1;
    unsigned int is_relocatable : 2;
    unsigned int is_entry : 1;
    unsigned int is_defined : 1;
    unsigned int is_used : 1;
    unsigned int line_number;
    unsigned int symbol_ID;
    struct symbol_t *next;
} symbol_t;

extern struct symbol_t *symbol_table_head;

/*binary code table*/
typedef struct
{
    unsigned int binary_code : 14;
    unsigned int is_symbol : 1;
    unsigned int is_data : 1;
} binary_t;

enum binary_type
{
    BINARY_INSTANT,
    BINARY_SYMBOL,
    BINARY_DATA,
    BINARY_STRING
};

/*adds a binary code to the binary table
 * @param ARE the 0-1 of the binary code
 * @param dest the 2-3 of the binary code
 * @param src the 4-5 of the binary code
 * @param opCode the 6-9 of the binary code
 * @param p1 the 10-11 of the binary code
 * @param p2 the 12-13 of the binary code
 */
void add_Binary(int ARE, int dest, int src, int opCode, int p1, int p2, int PC);

/*adds a binary code to the binary table
 * @param value the value of the binary code
 * @param type the type of the binary code
 * @param PC the current PC
 */
void add_Binary_var(int value, int type, int PC);

/* checks if a label is valid
 * @return true if valid
 * return false if not valid
 * @param label label to check
 */
int is_valid_label(char *label);

/* gets a symbol from the symbol table based on name
 * @return the symbol or NULL if not found
 * @param name the name of the symbol
 */
symbol_t *get_symbol(char *name);

/* gets a symbol the ID of the symbol based on name
 * @return the ID of the symbol or -1 if not found
 * @param name the name of the symbol
 */
unsigned int get_symbol_ID(char *name);

/* adds a symbol to the symbol table
 * @return the index of the symbol in the symbol table
 * @param name the name of the symbol
 * @param value the value of the symbol
 * @param is_data true if the symbol is a data symbol
 * @param is_relocatable true if the symbol is relocatable
 * @param is_entry true if the symbol is an entry symbol
 * @param is_defined true if the symbol is defined
 * @param is_used true if the symbol is used
 */
void add_symbol(char *name, int value, int is_data, int is_relocatable, int is_entry, int is_defined, int is_used);

/* deletes the symbol table */
void deletSymbolTable();

/* prints the symbol table for debug*/
void printTable();

/* prints the binary table for debug*/
void printBinary_t(int PC);

/* adds a binary code to the binary table
 * @param ARE the 0-1 of the binary code
 * @param dest the 2-3 of the binary code
 * @param src the 4-5 of the binary code
 * @param opCode the 6-9 of the binary code
 * @param p1 the 10-11 of the binary code
 * @param p2 the 12-13 of the binary code
 */
void add_Binary(int ARE, int dest, int src, int opCode, int p1, int p2, int PC);

/*adds a binary code to the binary table
 * @param value the value of the binary code
 * @param type the type of the binary code
 * @param PC the current PC
 */
void add_Binary_var(int value, int type, int PC);

/*adds a registers binary code to the binary table
 * @param srcValue the source register number
 * @param destValue the destination register number
 * @param PC the current PC
 */
void add_Binary_register(int srcValue, int destValue, int PC);

/*resets the binary table */
void deleteBinaryTable();

#endif /* symbol_table_h */
