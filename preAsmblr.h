#ifndef preAsmblr_h
#define preAsmblr_h

#include <stdio.h>

struct Macro; 

/* handles the command line arguments and opens the files
 * @return returns the name of the output file
 * @param argc - number of arguments 
 * @param argv - array of arguments 
 */
char* fileHandler(char* filename);

/* opens the original file and writes the output file with open macros
 * @return returns the name of the output file
 * @param output_file  the output file with open macros
 * @param file  the original file
 */
char *open_macro(FILE *output_file, FILE *file);

/* checks if the macro name is legal
 * @return returns true if name is legal
 * @param name  the name of the macro
 */
int legal_mcr_name(char *name);

#endif /* preAsmblr_h */