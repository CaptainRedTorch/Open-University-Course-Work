# Open-University-Course-Work
Open University Course Work 


made for an assignment(mmn14) at the open university of israel 
_____________________________________________________________

To compile and run the files:
 -use a terminal which supports gcc   
 -open the appropriate folder in terminal and use the make command
 -use the ./prog [file name] [file name...]  

To use the example files from the "input - output" folder please drag them into the same folder as prog
_____________________________________________________________

The program based on mmn14 guideline
The program takes a file with .as extension and assmbels it into files
.am file for displayed macros, .ob for machine code, .ext for external labels, .ent for entry labels

[file name] = enter the file name without the .as extenstion, you may enter more than one file name at once

For example: ./prog test test test2

Limitation: 
1. Lines in the .as file should not exceed 80 characters
2. The machine as 256 cells of memory
3. integer numbers for .data should be between 8191 to -8192
4. integer numbers for instant should be between 2047 to -2048
5. The machine as 8 registers r0 - r7
6. A string should consist of printable ascii characters
7. Number of Labels can't exceed 16383
8. The original File must have a .as extension
9. Saved words shouldn't be used as label names
______________________________________________________________

The reason the number of labels can't exceed 16383 is because under the guidelines spespications
there is no possible way in which the file could contain that number of labels and not exceed the memory capacity of the machine

Saved words:
The operation list: mov, cmp, add, sub, not, clr, lea, inc, dec, jmp, bne, red, prn, jsr, rts, stop
The directive list: .data, .string, .entry, .extern
The register list: r0, r1, r2, r3, r4, r5, r6, r7

_____________________________________________________________
the program files are stractured as:

main.c - using preAsmblr, firstPass, secondPass, constants, globals

firstPass.c - using helper_func, tables , constants, globals, errorHandle

secondPass.c - using tables, constans, globals, errorHandle

tables.c - using helper_func, constants, globals, errorHandle

helper_func.c - using constants

errorHandle.c 

preAsmblr.c - using macro_table, helper_func, constants, errorHandle

macro_table.c 

constants.h

globals.h - using tables, constants


