#ifndef secondPass_h
#define secondPass_h

/*
 * This function is the second pass of the assembler.
 * It gets a file name and returns 0 if the second pass was successful.
 * Otherwise, it returns an 1.
 */
int secondPass(char *filename);

/*
 * This function gets a binary code and returns the object code.
 * @param binary_code the binary code to translate
 * @param PC the program counter
 */
char *binaryToObject(int binary_code, int PC);

#endif /* secondPass_h */
