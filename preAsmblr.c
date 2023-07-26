#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "macro_table.h"
#include "preAsmblr.h"
#include "helper_func.h"
#include "constants.h"
#include "errorHandle.h"


char *fileHandler(char *argv)
{
    char *output_filename;
    char *input_filename = malloc(sizeof(char) * (strlen(argv) + 4));
    FILE *file, *output_file;

   
    sprintf(input_filename, "%s.as", argv);

    file = fopen(input_filename, "r");
    if (file == NULL)
    {
        printf("\nError: %s named \"%s\"", strerror(errno), input_filename);
        free(input_filename);
        return NULL;
    }
    free(input_filename);

    output_filename = malloc(sizeof(char) * (strlen(argv) + 4));
    sprintf(output_filename, "%s.am", argv);
    output_file = fopen(output_filename, "w");
    if (output_file == NULL)
    {
        printf("\nError: %s", strerror(errno));
        fclose(file);
        free (output_filename);
        return NULL;
    }
    if (open_macro(output_file, file) == NULL)
    {
        printf("\nError: in defining macros, .am file was not created\n");
        fclose(file);
        fclose(output_file);
        remove(output_filename);
        free(output_filename);
        return NULL;
    }

    fclose(file);
    fclose(output_file);
    free(output_filename);

    return argv;
}

char *open_macro(FILE *output_file, FILE *file)
{
    char line[MAX_LINE_LENGTH] = {'\0'};
    char *first_field;
    char *second_field;
    char *third_field;
    int isMCR = false;
    struct Macro *current_macro = NULL;
    struct Macro *found_macro = NULL;
    char cpy_line[MAX_LINE_LENGTH] = {'\0'};
    int line_number = 0;

    while (fgets(line, MAX_LINE_LENGTH, file) != NULL)
    {
        first_field = NULL;
        second_field = NULL;
        line_number++;
        strcpy(cpy_line, line);

        first_field = strtok(line, "\n\t\r ");
        if (first_field == NULL)
        {
            fprintf(output_file, "%s", cpy_line);
            continue;
        }

        /*if the first field is a macro from the marco table, then replace it*/
        if ((found_macro = find_macro(current_macro, first_field)) != NULL && isMCR == false)
        {
            char *replacement = get_replacement(found_macro);
            char *remaining_line = strtok(NULL, "\n"); /* get the rest of the line after the macro name*/
            clean_whitespace(&remaining_line);
            if (remaining_line == NULL)
            {
                /*there's nothing after the macro name, so just replace with the macro's replacement text*/
                fprintf(output_file, "%s", replacement);
            }
            else
            {
                /*not code can be after mcr (name)*/
                errorHandle(INVALID_MACRO_SYNATX, line_number, NULL, NULL);
            }
        }
        else if (strcmp(first_field, "mcr") == 0)
        {
            second_field = strtok(NULL, " \t\n\r");
            third_field = strtok(NULL, " \t\n\r");
            if (second_field == NULL) /*no name*/
            {
                errorHandle(INVALID_MACRO_NULL, line_number, NULL, NULL);
                return NULL;
            }
            if (find_macro(current_macro, second_field) != NULL) /*macro already exists*/
            {
                errorHandle(INVALID_MACRO_EXIST, line_number, NULL, second_field);
                return NULL;
            }
            if (third_field != NULL) /*additional code after macro name*/
            {
                errorHandle(INVALID_MACRO_SYNATX, line_number, NULL, NULL);
                return NULL;
            }
            if (legal_mcr_name(second_field) == false)/*macro name cant be reserved word*/
            {
                errorHandle(INVALID_MACRO_NAME, line_number, NULL, second_field);
                return NULL;
            }
            /*add macro name to macro table*/
            current_macro = add_macro(second_field, "");
            isMCR = true;
        }
        else if (isMCR)
        {
            /*replace macro*/
            char *remaning_line = strtok(NULL, "\n");
            clean_whitespace(&remaning_line);
            clean_whitespace(&first_field);
            if (strncmp(first_field, "endmcr", 6) != 0)
            {
                update_replacment(current_macro, cpy_line);
            }
            else
            {
                update_replacment(current_macro, "\0");
                isMCR = false;
            }
        }
        else
        {
            /*No macro, just output line*/
            fprintf(output_file, "%s", cpy_line);
        }
    }
    delete_macro_table(current_macro);
    fflush(output_file);
    return "success";
}

int legal_mcr_name(char *name)
{
    if (getDirectiveCode(name) != INVALID_DIRECTIVE || getOpcode(name) != INVALID_OPCODE)
        return false;
    if (is_register(name))
        return false;
    return true;
}
