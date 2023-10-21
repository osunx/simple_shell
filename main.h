#ifndef _MAIN_H_
#define _MAIN_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pwd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdbool.h>
#include <dirent.h>
#include <stddef.h>
#include <errno.h>
#include <ctype.h>
#include <regex.h>
#include <stdarg.h>
#include <sys/stat.h>
#include <fcntl.h>


#define BUFFER_SIZE 1024
#define BUF_NULLTERMINATE '\0'
#define MAX_PATH_LENGTH 1024



/**
 * struct Node - A structure to represent a linked list node.
 * @word: The word stored in the node.
 * @next: Pointer to the next node in the linked list.
 */
typedef struct Node
{
char *word;
struct Node *next;
} Node;

/* Prefix of the command ie execution command */
extern char *prefixes[];
char *startsWithPrefix(char *input);
char *concatTokens(char **tokens, char *separator);
void handle_errno(char *program_nam);

/* String functions prototype */
char *stringcpy(char *dest, const char *src);
int stringlen(const char *str);
int stringcmp(const char *s1, const char *s2);
char *stringcat(char *dest, const char *src);
char *stringtok(char *str, char *delim);

/* Morestring functions prototypes */
char *stringdup(const char *str);
int wputchar(int chars);
void wputs(char *str);
char *startwith(char *string, char *prefix);
size_t strexit(char *string, char *target);

/*Custom strtok function prototypes and a strncpy clone*/
void stringcpyall(char *dest, const char *src, size_t n);
char **stringsplit(const char *str, char *delim);
char *customstrtok(char *str, char *delim);
char *strpointbrk(const char *str, const char *charset);
bool is_integer(const char *str);

/*Enviroment create - env++ functions prototypes*/
char **create_environment(void);
char **set_environment(char *name, char *value, int overwrite, int mode);
int remove_environment(const char *name);
char *print_environment(char *mode);
void free_environment(char **env_array);

/*display prompt and Read command functions prototypes*/
int isInteractiveMode(void);
void displayHostName(void);
char *getUserName(void);
char *read_command();
char *strsearch(char *str, const char *keyword);

/* Knight functions prototypes*/
int isOnlyWhitespace(const char *str);
char **tokenize(char *str, char *delimiter);
char *stringarraycpy(char **arr);
size_t strcondition(char *String, char *stableString, char *allowedString, size_t occurence);
int containschars(const char *string, char *target);

/*Get path and execute command + system command prototype*/
char *get_command_path(char *command);
int execute_command(char *command);
void execute_cd(char *input);
int get_system(char *command);
int run_system(const char *command);

/*Menviroment getenv + more string funcion prototype*/
char *get_environment(const char *name);
int strinprintf(char *str, size_t size, const char *format, ...);
int stringtwocmp(const char *str1, const char *str2, size_t n);
void process_env_command(char *command);
void execute_commands_from_file(char *filename);

/*incrementString or write it*/
void* memoryset(void* ptr, int value, size_t num);
char* incrementString(char *numStr);

/* Built-in function prototypes */
void execute_exit(char *input);
void execute_env(void);
void execute_separator(char *input);
void execute_logical_operator(char *command);
int execute_with_variable_replacement(char *command);

/* Alias function prototypes */
size_t charsBeforeWord(char *string, char *word);
void addAlias(char *alias);
char *checkAlias(char *command);
char *cmdConstructor(char *command);
char **extractSubstrings(char *string, const char *keyword);


extern char **environ;
extern char **definedalias;
#endif /* MAIN_H */
