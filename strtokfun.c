#include "main.h"

/**
 * stringcpyall - Custom version of strncpy that copy a
 * specified number of characters from one string
 * to another ensures it's null-terminated.
 * @dest: Pointer to the destination buffer.
 * @src: Pointer to the source string.
 * @n: Number of characters to copy from source to destination.
 */

void stringcpyall(char *dest, const char *src, size_t n)
{
size_t i;
for (i = 0; i < n && src[i] != '\0'; ++i)
{
dest[i] = src[i];
}
for (; i < n; i++)
{
dest[i] = '\0';
}
}


/**
 * stringsplit - Splits a string into tokens based on a delimiter.
 *
 * @str: The input string to split.
 * @delim: The delimiter to use for splitting.
 *
 * Return: A dynamically allocated array of pointers to tokens
 * or NULL on failure.
 */

char **stringsplit(const char *str, char *delim)
{
char **tokens = NULL;
char *token, **new_tokens;
size_t token_count = 0;
size_t token_size = 10; 
char *str_copy;
size_t i;

if (str == NULL || delim == NULL)
{
perror("Invalid input");
return NULL;
}

/* Allocate memory for the initial tokens array */
tokens = (char **)malloc(token_size * sizeof(char *));
if (tokens == NULL)
{
perror("malloc");
return (NULL);
}

/* Create a copy of the input string to avoid modifying it */
str_copy = stringdup(str);
if (str_copy == NULL)
{
perror("strdup");
free(tokens); /* Free tokens before returningn */
free(str_copy); /* Free str_copy before returning */
return (NULL);
}

/* Tokenize the copied string using the specified delimiter */
token = customstrtok(str_copy, delim);
while (token != NULL)
{
if (token_count >= token_size)
{
/* Resize the tokens array if needed */
token_size *= 2;
new_tokens = (char **)realloc(tokens, token_size * sizeof(char *));
if (new_tokens == NULL)
{
perror("realloc");
free(str_copy); /* Free str_copy before returning */
for (i = 0; i < token_count; i++)
{
free(tokens[i]);
}
free(tokens); /* Free tokens before returning */
return (NULL);
}
tokens = new_tokens;
}

tokens[token_count] = stringdup(token);
if (tokens[token_count] == NULL)
{
perror("stringdup");
free(str_copy); /* Free str_copy before returning */
for (i = 0; i < token_count; i++)
{
free(tokens[i]);
}
free(tokens); /* Free tokens before returning */
return (NULL);
}

token_count++;
token = customstrtok(NULL, delim);
}

tokens[token_count] = NULL;
free(str_copy);

return (tokens);
}


/**
 * customstrtok - A custom implementation of strtok.
 *
 * @str: The input string to tokenize.
 * @delim: The delimiter string.
 *
 * Return: A pointer to the next token, or NULL if no more tokens are found.
 */

char *customstrtok(char *str, char *delim)
{
static char *last_token = NULL;
char *token;
/* If a new string is provided, update last_token */
if (str != NULL)
{
last_token = str;
}
else if (last_token == NULL)
{
return (NULL); /* No more tokens */
}
/* Skip leading delimiters */
while (*last_token && containschars(delim, last_token))
{
last_token++;
}

/* If at the end of the string, return NULL */
if (*last_token == '\0')
{
return (NULL);
}

token = last_token; /* Start of the token */

/* Find the end of the token */
last_token = strpointbrk(last_token, delim);
if (last_token != NULL)
{
*last_token = '\0'; /* Terminate the token */
last_token++; /* Move to the next character after the delimiter */
}
else
{
last_token = NULL; /* No more tokens */
}
return (token);
}

/**
 * strpointbrk - Search a string for any of a set of characters.
 *
 * @str: The input string to search.
 * @charset: The set of characters to search for.
 *
 * Return: A pointer to the first occurrence of a character from charset
 * in str, or NULL if no such character is found
 */

char *strpointbrk(const char *str, const char *charset)
{
/* Create a hash table to store characters from charset */
bool table[256] = {false}; /* Assuming ASCII characters */

/*Fill the hash table with characters from charset*/
while (*charset)
{
table[(unsigned char)(*charset)] = true;
charset++;
}

/*Search for characters from charset in the input string*/
while (*str)
{
if (table[(unsigned char)(*str)])
return ((char *)str); /*Found a character from charset*/
str++;
}

return (NULL); /* No character from charset found */
}


/**
 * is_integer - Check if a string is an integer.
 * @str: The string to check.
 *
 * Return: True if the string is an integer, false otherwise.
 */
bool is_integer(const char *str)
{
int i = 0;

if (str == NULL || *str == '\0')
{
return (false);
}

/* Handle negative numbers */
if (*str == '-')
{
str++;
}

for (i = 0; str[i] != '\0'; i++)
{
if (str[i] < '0' || str[i] > '9')
{
return (false);
}
}

return (true);
}
