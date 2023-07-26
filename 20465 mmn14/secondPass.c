#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "secondPass.h"
#include "globals.h"
#include "constants.h"
#include "tables.h"
#include "errorHandle.h"

extern int error_flag;
extern int PC;
extern int IC;
extern int DC;

int secondPass(char *filename)
{
    int i; /*for loop*/
    int binary_line; /*currect line number in ob file*/
    symbol_t *current_symbol = symbol_table_head;
    FILE *ob;
    FILE *ent;
    FILE *ext;
    char *ob_code;
    char *ent_name = malloc(sizeof(char) * (strlen(filename) + 5));
    char *ext_name = malloc(sizeof(char) * (strlen(filename) + 5));
    char *ob_name = malloc(sizeof(char) * (strlen(filename) + 5));
    sprintf(ent_name, "%s.ent", filename);
    sprintf(ext_name, "%s.ext", filename);
    sprintf(ob_name, "%s.ob", filename);

    /*check if there are undefined symbols*/
    while (current_symbol != NULL)
    {
        if (current_symbol->is_defined == false)
        {
            errorHandle(LABEL_NOT_DEFINED, current_symbol->line_number, NULL, current_symbol->name);
            error_flag = true;
        }
        /*checks if symbols were defined but not used*/
        if (current_symbol->is_used == false)
        {
            printf("\033[1;35m");
            printf("\n%d: Warning: ", current_symbol->line_number);
            printf("\033[0;37m");
            printf("label %s is not used", current_symbol->name);
            printf("\033[0m");
        }
        current_symbol = current_symbol->next;
    }

    /*if any errors where found dont create the files and exist*/
    if (error_flag == true)
    {
        deletSymbolTable();
        free(ent_name);
        free(ext_name);
        free(ob_name);
        return error_flag;
    }

    ob = fopen(ob_name, "w");
    fprintf(ob, "%d %d \n", IC, DC);
    ext = fopen(ext_name, "w");
    /*resolve the binary code for the symbols and write the instructions to object file */
    binary_line = 0;
    for (i = 0; i < PC - 100; i++)
    {
        if (binary_table[i].is_symbol == true)
        {
            symbol_t *current_symbol = symbol_table_head;

            while (current_symbol != NULL)
            {
                if (binary_table[i].binary_code == current_symbol->symbol_ID)
                {
                    /*write the .extern symbols to the external file*/
                    if (current_symbol->is_relocatable == false)
                    {
                        fprintf(ext, "%s\t%d\n", current_symbol->name, i + 100);
                    }
                    /*resolve and add the binary code for the symbol*/
                    binary_table[i].binary_code = (current_symbol->value << 2) | (((current_symbol->is_relocatable) ? 2 : 1) << 0);
                    fprintf(ob, "%s\n", ob_code = binaryToObject(binary_table[i].binary_code, binary_line++));
                    free(ob_code);
                    break;
                }

                current_symbol = current_symbol->next;
            }
        }
        else
        {
            /*write the instructions to the object file*/
            if (binary_table[i].is_data == false)
            {
                fprintf(ob, "%s\n", ob_code = binaryToObject(binary_table[i].binary_code, binary_line++));
                free(ob_code);
            }
        }
    }
    /*write the data to the object file*/
    for (i= 0; i < PC - 100; i++)
    {
        if (binary_table[i].is_data == true)
        {
            fprintf(ob, "%s\n", ob_code = binaryToObject(binary_table[i].binary_code, binary_line++));
            free(ob_code);
        }
    }

    /*write the .entry symbols to the entry file*/
    ent = fopen(ent_name, "w");
    current_symbol = symbol_table_head;
    while (current_symbol != NULL)
    {
        if (current_symbol->is_entry == true)
        {
            fprintf(ent, "%s\t%d\n", current_symbol->name, current_symbol->value);
        }
        current_symbol = current_symbol->next;
    }
    
    
    /*remove the entry and external files if they are empty*/
    if (ftell(ent) == 0)
    {
        remove(ent_name);
    }
    if (ftell(ext) == 0)
    {
        remove(ext_name);
    }

    fclose(ent);
    fclose(ext);
    fclose(ob);
    deletSymbolTable();

    free(ent_name);
    free(ext_name);
    free(ob_name);
    return error_flag;
}

/*function to conver from 0 and 1 to . and / */
char *binaryToObject(int binary_code, int PC)
{
    char *object_code = (char *)malloc(sizeof(char) * 20);
    int i;
    sprintf(object_code, "%04d\t", PC + 100);
    for (i = 13; i >= 0; i--)
    {
        if (binary_code & (1 << i))
        {
            strcat(object_code, "/");
        }
        else
        {
            strcat(object_code, ".");
        }
    }
    object_code[19] = '\0';
    return object_code;
}
