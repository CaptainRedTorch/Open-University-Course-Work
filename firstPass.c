#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

#include "firstPass.h"
#include "helper_func.h"
#include "tables.h"
#include "constants.h"
#include "globals.h"
#include "errorHandle.h"

/*global variables*/
int error_flag = false;
extern binary_t binary_table[MAX_FILE_LENGTH];
int line_number = 0;
int IC = 0;
int DC = 0;
int PC = 100;
int firstPass(char *filename)
{

    int is_label = false;
    FILE *file;
    char *line = (char *)malloc(sizeof(char) * MAX_LINE_LENGTH);
    char *line_cpy = (char *)malloc(sizeof(char) * MAX_LINE_LENGTH);
    char *label_name = (char *)malloc(sizeof(char) * MAX_LINE_LENGTH);
    char *output_filename = malloc(sizeof(char) * (strlen(filename) + 5));

    memset(line, '\0', MAX_LINE_LENGTH * sizeof(char));
    memset(output_filename, '\0', (strlen(filename) + 5) * sizeof(char));

    /*reset gloabals between files*/

    sprintf(output_filename, "%s.am", filename);
    file = fopen(output_filename, "r");

    if (file == NULL || ferror(file) || feof(file))
    {
        printf("\nError: %s", strerror(errno));
        error_flag = true;

        fclose(file);
        free(line);
        free(output_filename);
        return error_flag;
    }

    /*get the next line from the file (2.)*/
    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        int dircetCode;
        char *token;
        int label_statues = INVALID_LABEL_NULL;

        line_number++;

        memset(label_name, '\0', MAX_LINE_LENGTH * sizeof(char));
        memset(line_cpy, '\0', MAX_LINE_LENGTH * sizeof(char));
        strcpy(line_cpy, line);

        if (line[strlen(line) - 1] != '\n' && !feof(file))
        {
            errorHandle(INVALID_LINE_LENGTH, line_number, line_cpy, NULL);
            error_flag = true;
            while(fgets(line, MAX_LINE_LENGTH, file) != NULL)
            {
                if (line[strlen(line) - 1] == '\n')
                {
                    break;
                }
            }
            continue;
        }
        token = strtok(line, " \t");
        /*Check if the line is empty*/
        if (token == NULL)
        {
            continue;
        }

        /*Check if the line is a comment*/
        if (token[0] == ';')
        {
            continue;
        }

        clean_whitespace(&token);
        if (token[0] == '\0' || strlen(token) == 0)
        {
            continue;
        }


        /*Check if the first field is a label (3.)*/
        if (token[strlen(token) - 1] == ':')
        {
            strncpy(label_name, token, MAX_LINE_LENGTH);
            label_name[strlen(label_name) - 1] = '\0';
            if ((label_statues = is_valid_label(label_name)) != true)
            {
                errorHandle(label_statues, line_number, line_cpy, label_name);
                error_flag = true;
                continue;
            }
            /*turn on is_label (4.)*/
            is_label = true;

            token = strtok(NULL, " \t\n");
            clean_whitespace(&token);
        }
        else
        {
            is_label = false;
        }

        dircetCode = getDirectiveCode(token);
        /*check if .data/.string (5.)*/
        switch (dircetCode)
        {
        case DATA:
        {
            /*if is_label on add label (6.)*/
            if (is_label == true)
            {
                add_symbol(label_name, PC, true, true, false, true, false);
            }
            /*recognize data type encode in memory, update DC (7.)*/
            do
            {
                /* TODO check empty number, check end of numbers, check missing numbers */
                token = strtok(NULL, ",");
                clean_whitespace(&token);
                if (token != NULL)
                {
                    if (is_number(token) == true)
                    {
                        add_Binary_var(atoi(token), BINARY_DATA, PC++);
                        DC++;
                    }
                    else
                    {
                        if (strlen(token) < 1)
                        {
                            errorHandle(INVALID_DATA_SYNTAX, line_number, line_cpy, NULL);
                        }
                        else
                        {
                            errorHandle(INVALID_DATA, line_number, line_cpy, token);
                        }
                        error_flag = true;
                    }
                }
            } while (token != NULL);

            break;
        }
        case STRING:
        {
            if (is_label == true)
            {
                add_symbol(label_name, PC, true, true, false, true, false);
            }

            /*recognize data type encode in memory, update DC (7.)*/
            token = strtok(NULL, "\n");
            clean_whitespace(&token);
            if (token == NULL)
            {
                errorHandle(INVALID_STRING_NULL, line_number, line_cpy, token);
                error_flag = true;
            }
            else
            {
                if (is_string(token) == false)
                {
                    errorHandle(INVALID_STRING, line_number, line_cpy, token);
                    error_flag = true;
                }
                else
                {
                    int i;
                    for (i = 1; i < strlen(token) - 1; i++)
                    {
                        add_Binary_var(token[i], BINARY_STRING, PC++);
                    }
                    add_Binary_var(0, BINARY_STRING, PC++);
                    DC += strlen(token) - 1;
                }
            }
            break;
        }

        /* check if .extern/.entry (8.)*/
        case EXTERN:
        {
            /*if is_label on add label (9.)*/
            if (is_label == true)
            {
                add_symbol(label_name, 0, false, false, false, true, false);
            }
            /*if .extern add symbol*/
            do
            {
                token = strtok(NULL, ",\n");
                clean_whitespace(&token);
                if (token != NULL)
                {
                    if ((label_statues = is_valid_label(token)) != true)
                    {
                        errorHandle(label_statues, line_number, line_cpy, token);
                        error_flag = true;
                        break;
                    }
                    else
                    {
                        add_symbol(token, 0, false, false, false, true, false);
                    }
                }
            } while (token != NULL);
            break;
        }
        case ENTRY:
        {
            if (is_label == false)
            {
                token = strtok(NULL, ",\n");
                clean_whitespace(&token);
                if (token != NULL)
                {
                    if ((label_statues = is_valid_label(token)) != true)
                    {
                        errorHandle(label_statues, line_number, line_cpy, token);
                        error_flag = true;
                        break;
                    }
                    else
                    {
                        add_symbol(token, 0, false, true, true, false, false);
                    }
                }
            }
            break;
        }
        /*instruction or invalid command*/
        case INVALID_DIRECTIVE:
        {
            int LC = 0;     /*LC - counts number of words in a line for instructions*/
            int opCode;
            /*if is_label on add label to table*/
            if (is_label == true)
            {
                add_symbol(label_name, PC, false, true, false, true, false);
            }
            /*recognize command type*/
            opCode = getOpcode(token);

            /*invalid command*/
            if (opCode == INVALID_OPCODE)
            {
                errorHandle(INVALID_COMMAND, line_number, line_cpy, token);
                error_flag = true;
                continue;
            }
            /*recognize command operands*/
            /*encode command in memory*/
            if ((LC = opCode_analizer(opCode, strtok(NULL, "\n"), LC, PC, line_cpy)) == -1) /*LC == -1 , if line is invalid*/
            {
                LC = 0;
                error_flag = true;
                continue;
            }
            else
            {
                /*update IC*/
                IC += (LC - PC) + 1;
                PC = LC + 1;
            }
            break;
        }
        } /*end of switch case*/

    } /*end of while read lines*/

    /*reset and close files*/
    fclose(file);
    free(output_filename);
    free(line);
    free(line_cpy);
    free(label_name);
    return error_flag;
}

/*function to analize instruction lines*/
int opCode_analizer(int opCode, char *token, int LC, int PC, char *line_cpy)
{
    int tempPC = PC; /*PC for first instruction in line*/
    char *src;
    char *dest;
    int srcType = 0;
    int destType = 0;
    /*mov/add/sub*/
    if (opCode == 0 || opCode == 2 || opCode == 3)
    {
        src = strtok_single(token, ",");
        clean_whitespace(&src);
        dest = strtok_single(NULL, "\0");
        clean_whitespace(&dest);
        if (src == NULL || dest == NULL)
        {
            errorHandle(INVALID_MISSING_OPERAND, line_number, line_cpy, NULL);
            error_flag = true;
            return -1;
        }

        if (is_register(src))
        {
            srcType = REGISTER;
            add_Binary_register(register_number(src), 0, ++PC);
        }
        else if (is_instant(src))
        {
            srcType = INSTANT;
            add_Binary_var(atoi(src + 1), BINARY_INSTANT, ++PC);
        }
        else if (is_valid_label(src) == true)
        {
            add_symbol(src, PC, false, true, false, false, true);
            srcType = LABEL;
            add_Binary_var(get_symbol_ID(src), BINARY_SYMBOL, ++PC);
        }
        else
        {
            errorHandle(INVALID_OPERAND_SRC, line_number, line_cpy, src);
            error_flag = true;
            return -1;
        }

        if (is_register(dest))
        {
            destType = REGISTER;
            add_Binary_register(0, register_number(dest), ++PC);
        }
        else if (is_valid_label(dest) == true)
        {
            add_symbol(dest, PC, false, true, false, false, true);
            destType = LABEL;
            add_Binary_var(get_symbol_ID(dest), BINARY_SYMBOL, ++PC);
        }
        else
        {
            if (strchr(dest, ',') != NULL)
                errorHandle(INVALID_EXTRA_OPERAND, line_number, line_cpy, NULL);
            else
                errorHandle(INVALID_OPERAND_DEST, line_number, line_cpy, dest);
            error_flag = true;
            return -1;
        }

        if (destType == REGISTER && srcType == REGISTER)
        {
            /*delete the last register and add it to the one before it*/
            add_Binary(0, destType, srcType, opCode, 0, 0, tempPC);
            binary_table[PC--].binary_code = 0;
            add_Binary_register(register_number(src), register_number(dest), PC);
            return PC;
        }
        else
        {
            add_Binary(0, destType, srcType, opCode, 0, 0, tempPC);
            return PC;
        }
    }
    /*cmp*/
    else if (opCode == 1)
    {
        src = strtok_single(token, ",");
        clean_whitespace(&src);
        dest = strtok_single(NULL, "\n");
        clean_whitespace(&dest);
        if (src == NULL || dest == NULL)
        {
            errorHandle(INVALID_MISSING_OPERAND, line_number, line_cpy, NULL);
            error_flag = true;
            return -1;
        }

        if (is_register(src))
        {
            srcType = REGISTER;
            add_Binary_register(register_number(src), 0, ++PC);
        }
        else if (src[0] == '#' && is_number(src + 1))
        {
            srcType = INSTANT;
            add_Binary_var(atoi(src + 1), BINARY_INSTANT, ++PC);
        }
        else if (is_valid_label(src)==true)
        {
            add_symbol(src, PC, false, true, false, false, true);
            srcType = LABEL;
            add_Binary_var(get_symbol_ID(src), BINARY_SYMBOL, ++PC);
        }
        else
        {
            errorHandle(INVALID_OPERAND_SRC, line_number, line_cpy, src);
            error_flag = true;
            return -1;
        }

        if (is_register(dest))
        {
            destType = REGISTER;
            add_Binary_register(0, register_number(dest), ++PC);
        }
        else if (is_valid_label(dest) == true)
        {
            add_symbol(dest, PC, false, true, false, false, true);
            destType = LABEL;
            add_Binary_var(get_symbol_ID(dest), BINARY_SYMBOL, ++PC);
        }
        else if (dest[0] == '#' && is_number(dest + 1))
        {
            destType = INSTANT;
            add_Binary_var(atoi(dest + 1), BINARY_INSTANT, ++PC);
        }
        else
        {
            if (strchr(dest, ',') != NULL)
                errorHandle(INVALID_EXTRA_OPERAND, line_number, line_cpy, NULL);
            else
                errorHandle(INVALID_OPERAND_DEST, line_number, line_cpy, dest);
            error_flag = true;
            return -1;
        }

        if (destType == REGISTER && srcType == REGISTER)
        {
            /*delete the last register and add it to the one before it*/
            add_Binary(0, destType, srcType, opCode, 0, 0, tempPC);
            binary_table[PC--].binary_code = 0;
            add_Binary_register(register_number(src), register_number(dest), PC);
            return PC;
        }
        else
        {
            add_Binary(0, destType, srcType, opCode, 0, 0, tempPC);
            return PC;
        }
    }
    /*lea*/
    else if (opCode == 6)
    {
        src = strtok_single(token, ",");
        clean_whitespace(&src);
        dest = strtok_single(NULL, "\n");
        clean_whitespace(&dest);
        if (src == NULL || dest == NULL)
        {
            errorHandle(INVALID_MISSING_OPERAND, line_number, line_cpy, NULL);
            error_flag = true;
            return -1;
        }

        if (is_register(src))
        {
            errorHandle(INVALID_OPERAND_SRC, line_number, line_cpy, src);
            error_flag = true;
            return -1;
        }
        else if (is_instant(src) == true)
        {
            errorHandle(INVALID_OPERAND_SRC, line_number, line_cpy, src);
            error_flag = true;
            return -1;
        }
        else if (is_valid_label(src) ==true)
        {
            add_symbol(src, PC, false, true, false, false, true);
            srcType = LABEL;
            add_Binary_var(get_symbol_ID(src), BINARY_SYMBOL, ++PC);
        }
        else
        {
            if (strchr(src, ',') != NULL)
                errorHandle(INVALID_EXTRA_OPERAND, line_number, line_cpy, NULL);
            else
                errorHandle(INVALID_OPERAND_SRC, line_number, line_cpy, src);
            error_flag = true;
            return -1;
        }

        if (is_register(dest))
        {
            destType = REGISTER;
            add_Binary_register(0, register_number(dest), ++PC);
        }
        else if (is_valid_label(dest) == true)
        {
            add_symbol(dest, PC, false, true, false, false, true);
            destType = LABEL;
            add_Binary_var(get_symbol_ID(dest), BINARY_SYMBOL, ++PC);
        }
        else
        {
            if (strchr(dest, ',') != NULL)
                errorHandle(INVALID_EXTRA_OPERAND, line_number, line_cpy, NULL);
            else
                errorHandle(INVALID_OPERAND_DEST, line_number, line_cpy, dest);
            error_flag = true;
            return -1;
        }

        if (destType == REGISTER && srcType == REGISTER)
        {
            /*delete the last register and add it to the one before it*/
            add_Binary(0, destType, srcType, opCode, 0, 0, tempPC);
            binary_table[PC--].binary_code = 0;
            add_Binary_register(register_number(src), register_number(dest), PC);
            return PC;
        }
        else
        {
            add_Binary(0, destType, srcType, opCode, 0, 0, tempPC);
            return PC;
        }

        add_Binary(0, destType, srcType, opCode, 0, 0, tempPC);
        return PC;
    }
    /*not/clr/inc/dec/red*/
    else if (opCode == 4 || opCode == 5 || opCode == 7 || opCode == 8 || opCode == 11)
    {
        dest = strtok_single(token, "\n");
        clean_whitespace(&dest);
        if (dest == NULL)
        {
            errorHandle(INVALID_MISSING_OPERAND, line_number, line_cpy, NULL);
            error_flag = true;
            return -1;
        }

        if (is_register(dest))
        {
            destType = REGISTER;
            add_Binary_register(0, register_number(dest), ++PC);
        }
        else if (is_valid_label(dest) == true)
        {
            add_symbol(dest, PC, false, true, false, false, true);
            destType = LABEL;
            add_Binary_var(get_symbol_ID(dest), BINARY_SYMBOL, ++PC);
        }
        else
        {
            if (strchr(dest, ',') != NULL)
                errorHandle(INVALID_EXTRA_OPERAND, line_number, line_cpy, NULL);
            else
                errorHandle(INVALID_OPERAND_DEST, line_number, line_cpy, dest);
            error_flag = true;
            return -1;
        }

        add_Binary(0, destType, 0, opCode, 0, 0, tempPC);
        return PC;
    }
    /*jmp/bne/jsr*/
    else if (opCode == 9 || opCode == 10 || opCode == 13)
    {
        int relative = 0;
        int p1_type = 0, p2_type = 0;
        dest = strtok_single(token, "\n");
        clean_whitespace(&dest);
        if (dest == NULL)
        {
            errorHandle(INVALID_MISSING_OPERAND, line_number, line_cpy, NULL);
            error_flag = true;
            return -1;
        }

        if (is_instant(dest))
        {
            errorHandle(INVALID_OPERAND_DEST, line_number, line_cpy, dest);
            error_flag = true;
            return -1;
        }
        else if (is_register(dest))
        {
            destType = REGISTER;
            add_Binary_register(0, register_number(dest), ++PC);
        }
        else if (is_valid_label(dest) == true )
        {
            add_symbol(dest, PC, false, true, false, false, true);
            destType = LABEL;
            add_Binary_var(get_symbol_ID(dest), BINARY_SYMBOL, ++PC);
        }
        else if ((relative = is_relative(dest, PC, &p1_type, &p2_type, line_cpy)) != -1)
        {
            destType = RELATIVE;
            PC = relative;
        }
        else
        {
            error_flag = true;
            return -1;
        }

        add_Binary(0, destType, 0, opCode, p2_type, p1_type, tempPC);
        return PC;
    }
    /*prn*/
    else if (opCode == 12)
    {
        dest = strtok_single(token, "\n");
        clean_whitespace(&dest);
        if (dest == NULL)
        {
            errorHandle(INVALID_MISSING_OPERAND, line_number, line_cpy, NULL);
            error_flag = true;
            return -1;
        }

        if (is_instant(dest))
        {
            destType = INSTANT;
            add_Binary_var(atoi(dest + 1), BINARY_INSTANT, ++PC);
        }
        else if (is_register(dest))
        {
            destType = REGISTER;
            add_Binary_register(0, register_number(dest), ++PC);
        }
        else if (is_valid_label(dest) == true)
        {
            add_symbol(dest, PC, false, true, false, false, true);
            destType = LABEL;
            add_Binary_var(get_symbol_ID(dest), BINARY_SYMBOL, ++PC);
        }
        else
        {
            if (strchr(dest, ',') != NULL)
                errorHandle(INVALID_EXTRA_OPERAND, line_number, line_cpy, NULL);
            else
                errorHandle(INVALID_OPERAND_SRC, line_number, line_cpy, dest);
            error_flag = true;
            return -1;
        }
        add_Binary(0, destType, 0, opCode, 0, 0, tempPC);
        return PC;
    }
    /*rts/stop*/
    else if (opCode == 14 || opCode == 15)
    {
        dest = strtok_single(token, "\n");
        clean_whitespace(&dest);
        if (dest != NULL)
        {
            errorHandle(INVALID_EXTRA_OPERAND, line_number, line_cpy, NULL);
            error_flag = true;
            return -1;
        }
        add_Binary(0, 0, 0, opCode, 0, 0, tempPC);
        return PC;
    }

    return -1;
}

/*function to analize type 2 relative operands*/
int is_relative(char *str, int PC, int *p1_type, int *p2_type, char *line_cpy)
{
    char *label_name = strtok_single(str, "(");
    char *remaning = strtok_single(NULL, "\n\0");
    if (label_name == NULL && remaning == NULL)
    {
        errorHandle(INVALID_MISSING_OPERAND, line_number, line_cpy, NULL);
        return -1;
    }
    else if ((label_name == NULL || (int)label_name[0] == '\0') && remaning != NULL)
    {
        errorHandle(INVALID_RELATIVE, line_number, line_cpy, NULL);
        return -1;
    }
    else if ((is_valid_label(label_name) != true) && remaning != NULL)
    {
        errorHandle(INVALID_RELATIVE_LABEL, line_number, line_cpy, label_name);
        return -1;
    }
    else if (remaning == NULL)
    {
        errorHandle(INVALID_OPERAND, line_number, line_cpy, NULL);
        return -1;
    }
    else
    {
        add_symbol(label_name, PC, false, true, false, false, true);
        add_Binary_var(get_symbol_ID(label_name), BINARY_SYMBOL, ++PC);
    }

    if (remaning[strlen(remaning) - 1] == ')')
    {

        char *p1 = strtok_single(remaning, ",");
        char *p2 = strtok_single(NULL, ")");
        *p1_type = -1;
        *p2_type = -1;

        if (p1 == NULL || p2 == NULL)
        {
            errorHandle(INVALID_MISSING_OPERAND, line_number, line_cpy, NULL);
            return -1;
        }
        if (is_instant(p1) == true)
        {
            *p1_type = INSTANT;
            /*encode the instant*/
            add_Binary_var(atoi(p1 + 1), BINARY_INSTANT, ++PC);
        }
        else if (is_register(p1) == true)
        {
            *p1_type = REGISTER;
            /*encode the register*/
            add_Binary_register(register_number(p1), 0, ++PC);
        }
        else if (is_valid_label(p1) == true)
        {
            *p1_type = LABEL;
            add_symbol(p1, PC, false, true, false, false, true);
            /*encode the label*/
            add_Binary_var(get_symbol_ID(p1), BINARY_SYMBOL, ++PC);
        }
        else
        {
            errorHandle(INVALID_OPERAND_SRC, line_number, line_cpy, p1);
            return -1;
        }

        if (is_instant(p2) == true)
        {
            *p2_type = INSTANT;
            /*encode the instant*/
            add_Binary_var(atoi(p2 + 1), BINARY_INSTANT, ++PC);
        }
        else if (is_register(p2) == true)
        {
            *p2_type = REGISTER;
            /*encode the register*/
            add_Binary_register(0, register_number(p2), ++PC);
        }
        else if (is_valid_label(p2) == true)
        {
            *p2_type = LABEL;
            add_symbol(p2, PC, false, true, false, false, true);
            /*encode the label*/
            add_Binary_var(get_symbol_ID(p2), BINARY_SYMBOL, ++PC);
        }
        else
        {
            errorHandle(INVALID_OPERAND_DEST, line_number, line_cpy, p2);
            return -1;
        }

        if ((*p1_type == INSTANT && *p2_type == REGISTER) || (*p1_type == REGISTER && *p2_type == INSTANT))
            return PC;
        else if ((*p1_type == LABEL && *p2_type == INSTANT) || (*p1_type == INSTANT && *p2_type == LABEL))
            return PC;
        else if ((*p1_type == REGISTER && *p2_type == LABEL) || (*p1_type == LABEL && *p2_type == REGISTER))
            return PC;
        else if (*p1_type == LABEL && *p2_type == LABEL)
            return PC;
        else if (*p1_type == REGISTER && *p2_type == REGISTER)
        {
            /*delete the last register and add it to the other one before it*/
            binary_table[PC--].binary_code = 0;
            add_Binary_register(register_number(p1), register_number(p2), PC);
            return PC;
        }
        else
        {
            errorHandle(INVALID_RELATIVE, line_number, line_cpy, NULL);
            return -1;
        }
    }
    else
        return -1;
}
