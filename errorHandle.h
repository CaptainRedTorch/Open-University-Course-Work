
#ifndef errorHandle_h
#define errorHandle_h

/*error guide*/
enum errorsPass1{
    LABEL_ENTRY_EXTERN = 2,
    LABEL_DEFINED_ENTRY,
    LABEL_NOT_DEFINED,
    LABEL_NOT_FOUND,
    LABEL_ALREADY_EXISTS,
    INVALID_LABEL_NAME,
    INVALID_LABEL_LENGTH,
    INVALID_LABEL_NULL,
    INVALID_LABEL_RESERVED,
    INVALID_LABEL_FIRST,
    INVALID_STRING,
    INVALID_STRING_NULL,
    INVALID_DATA,
    INVALID_DATA_SYNTAX,
    INVALID_COMMAND,
    INVALID_MISSING_OPERAND,
    INVALID_EXTRA_OPERAND,
    INVALID_OPERAND_DEST,
    INVALID_OPERAND_SRC,
    INVALID_MACRO_NAME,
    INVALID_MACRO_NULL,
    INVALID_MACRO_EXIST,
    INVALID_MACRO_SYNATX,
    INVALID_LINE_LENGTH,
    INVALID_RELATIVE_LABEL,
    INVALID_RELATIVE,
    INVALID_OPERAND
};


/*
    * prints the error messages
    * @param error error code
    * @param line_number line number of error
    * @param line line of error
    * @param token the cause of the error
*/
void errorHandle(int error, int line_number, char *line, char* token);

#endif /* errorHandle_h */