#include <stdlib.h>
#include <string.h>
#include  <stdio.h>

#include "macro_table.h"

struct Macro* macro_table = NULL;


char* get_replacement(struct Macro* mcr) {
    if (mcr != NULL) {
        return mcr->replacement;
    }
    return NULL;
}

char* get_name(struct Macro* mcr) {
    if (mcr != NULL) {
        return mcr->name;
    }
    return NULL;
}

struct Macro* get_next(struct Macro* mcr) {
    if (mcr != NULL) {
        return mcr->next;
    }
    return NULL;
}

struct Macro *add_macro(char* name, char* replacement) {
    struct Macro* new_macro = (struct Macro*) malloc(sizeof(struct Macro));
    new_macro->name = (char*)malloc(strlen(name) + 1);
    strcpy((new_macro->name), name);
    new_macro->replacement = (char*)malloc(strlen(replacement) + 1);
    strcpy((new_macro->replacement), replacement);
    new_macro->next = macro_table;
    macro_table = new_macro;
    return new_macro;
}

void delete_macro_table(struct Macro* current) {
    
   while(current != NULL) {
       struct Macro* next = current->next;
       free(current->name);
       free(current->replacement);
       free(current);
       current = next;
   }
    
}

struct Macro* find_macro(struct Macro* current ,char* first_field) {
    
    if (current == NULL) {
       return NULL;
    }
    else
    {
        if (strcmp(current->name, first_field) == 0) {
            return current;
        }
        return find_macro(get_next(current), first_field);
    }
}

void update_replacment( struct Macro* mcr, char* replacement) {
    struct Macro* macro = mcr;
    char* combined_text;
    if (macro != NULL) 
    {
        if (macro->replacement == NULL) {
            macro->replacement = replacement;
            return;
        }
        combined_text = (char*)malloc(strlen(macro->replacement) + strlen(replacement) + 1);
        strcpy(combined_text, macro->replacement);
        strcat(combined_text, replacement);
        free(macro->replacement);
        macro->replacement = combined_text;
    }
}

