#ifndef firstPass_h
#define firstPass_h


/*
* first pass of the assembler
* @param filename  the name of the file to be assembled
* @return 1 if success, 0 if error
*/
int firstPass(char *filename);

/* analize the opCode and return the number of words that the command needs
* @param opCode the opCode of the command
* @param token the rest of the line
* @param LC the line counter
* @param IC the instruction counter
*/
int opCode_analizer(int opCode, char *token , int LC, int IC, char* line_cpy);

/* checks if string is a relative and analizes it and adds it to the binary table 
* Ex. FUNC(r1,r2)
* @return updated PC if string is a relative
* return -1 if string is not a relative
* @param str string to check
*/
int is_relative(char* str, int PC, int* p1_type, int* p2_type, char* line_cpy);


#endif /* firstPass_h */


