#ifndef macro_table_h
#define macro_table_h

/*macro_table.h*/

struct Macro {
    char* name;
    char* replacement;
    struct Macro* next;
};

/*@return the replacement text of the macro
* returns NULL if the macro is NULL
* @param mcr  Pointer to the macro
*/
char* get_replacement(struct Macro* mcr);

/*@return the name of the macro
* returns NULL if the macro is NULL
* @param mcr  Pointer to the macro
*/
char* get_name(struct Macro* mcr);

/*@return the macro adress with the given name
* returns NULL if the macro is NULL
* @param current  Pointer to the top of the list
* @param name  the name of the macro
*/
struct Macro* find_macro(struct Macro* current, char* name);

/*updates the replacement text of the macro
* @param mcr  Pointer to the macro
* @param replacement  the new replacement text
*/
void update_replacment( struct Macro* mcr, char* replacement) ;

/*adds a new macro to the macro table
* @param name  the name of the macro
* @param replacement  the replacement text of the macro
* @return returns the adress of the new macro
*/
struct Macro* add_macro(char* name, char* replacement);

/*deletes the macro table
* @param current  Pointer to the top of the list
*/
void delete_macro_table(struct Macro* current);

/*@return the next macro in the list
* returns NULL if the macro is NULL
* @param mcr  Pointer to the macro
*/
struct Macro* get_next(struct Macro* mcr);

#endif /* macro_table_h */