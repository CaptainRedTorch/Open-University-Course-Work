#ifndef globals_h
#define globals_h

#include "tables.h"
#include "constants.h"

extern binary_t binary_table[MAX_FILE_LENGTH]; /*table to hold the machine code*/
extern int line_number; /*current line*/
extern int error_flag; /*if true errors was found*/
extern int IC; /*instructions counter*/
extern int DC; /*data counter*/
extern int PC; /*overall counter*/

#endif /* globals_h */