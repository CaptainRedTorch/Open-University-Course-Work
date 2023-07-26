#ifndef constants_h
#define constants_h

/* adressing modes */
#define INSTANT 0
#define LABEL 1
#define RELATIVE 2
#define REGISTER 3

#define true 1
#define false 0

/*The maximum memory*/
#define MAX_FILE_LENGTH 256
/*The maximum legth of a label asin the guidelines is 30 + null terminator*/
#define MAX_LABEL_LENGTH 31

/* 14 bit intege */
#define MAX_INTEGER_14 8191
#define MIN_INTEGER_14 -8192

/* 12 bit integer*/
#define MAX_INTEGER_12 2047
#define MIN_INTEGER_12 -2048

#define MAX_LINE_LENGTH 80


#endif /* constants_h */
