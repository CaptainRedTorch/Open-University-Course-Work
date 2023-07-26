#ifndef helper_func_h
#define helper_func_h

/*Define opcodes as an enumeration*/
enum Opcode {
    MOV,
    CMP,
    ADD,
    SUB,
    NOT,
    CLR,
    LEA,
    INC,
    DEC,
    JMP,
    BNE,
    RED,
    PRN,
    JSR,
    RTS,
    STOP,
    INVALID_OPCODE
};

/*Define directives as an enumeration*/
enum DirectCode {
    DATA,
    STRING,
    ENTRY,
    EXTERN,
    INVALID_DIRECTIVE
};


/* compres string to directive names
* @return directiveCode if match
* return INVALID_DIRECTIVE if no matchs
* @param name string to compare
*/
unsigned int getDirectiveCode(char *name);

/* compres string to instruction names
* @return opCode if match
* return INVALID_OPCODE if no matchs
* @param name string to compare
*/
unsigned int getOpcode(char *opname);

/* strtok function to seperate even consecutive delimiters into tokens
* @return the token or NULL if no token is found
* @param str string to tokenize
*/
char* strtok_single(char* str, const char* delim);

/* removes spaces from the end and start of token
* @param str string to clean
*/
void clean_whitespace(char** str_ptr);

/* checks if string is a register
* @return true if string is a register
* return false if string is not a register
* @param str string to check
*/
int is_register(char *str);

/* checks if string is a number
* @return true if string is a number
* return false if string is not a number
* @param str string to check
*/
int is_number(char* str);

/* checks if string is a instant Ex. #5
* @return true if string is a instant
* return false if string is not a instant
* @param str string to check
*/
int is_instant(char* str);

/* return the registers number
* @return the registers number
* @param the register string to check extract number from
*/
int register_number(char *str);

/* checks if string is a "string"
* @return true if string is a string
* return false if string is not a string
* @param str string to check
*/
int is_string(char *str);

#endif /* helper_func_h */
