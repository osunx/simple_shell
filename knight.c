#include "main.h"


/**
 * isOnlyWhitespace - Check if a string consists of only whitespace characters.
 * @str: The input string to check.
 *
 * Return: 1 if the string contains only whitespace, 0 otherwise.
 */
int isOnlyWhitespace(const char *str)
{
	if (str == NULL)
	{
	return (0); /* NULL strings are not considered whitespace-only */
	}

	while (*str)
	{
	if (isspace(*str))
	{
		return (0); /* Found a non-whitespace character */
	}
		str++;
	}

	return (1); /* All characters are whitespace */
}



/**
 * stringarraycpy - Concatenate an array of strings into a single string.
 * @arr: The array of strings to concatenate.
 *
 * Return: The concatenated string on success, or NULL on failure.
 */
char *stringarraycpy(char **arr) {
    size_t total_length = 0;
    size_t num_strings = 0;
    size_t i;
    char *result;

    if (arr == NULL) {
        return (NULL); /* Return NULL for an empty array */
    } 

    /* Calculate the total length and the number of strings in the array */
    for (i = 0; arr[i] != NULL; i++) {
        total_length += strlen(arr[i]);
        num_strings++;
    }

    /* Allocate memory for the concatenated string */
    result = (char *)malloc(total_length + num_strings); /* +num_strings for space between strings */

    if (result == NULL) {
        return (NULL); /* Memory allocation failed */
    }

    result[0] = '\0'; /* Initialize the result string as an empty string */

    /* Concatenate the strings with spaces in between */
    for (i = 0; arr[i] != NULL; i++) {
        stringcat(result, arr[i]);
        if (i < num_strings - 1) {
            stringcat(result, " ");
        }
    }

    return (result);
}


/**
 * tokenize - Tokenize a string using a delimiter.
 * @str: The string to tokenize.
 * @delimiter: The delimiter to use for tokenization.
 *
 * Return: An array of tokens (strings), or NULL on failure.
 */

char **tokenize(const char *str, char *delimiter) {
    char *str_copy = stringdup(str); /* Create a copy of the input string */
    char *token;
    char **tokens = NULL;
    size_t count = 0;
    size_t i;

        if (!str_copy) {
        perror("strdup");
        return (NULL);
    }

    /* Tokenize the string using strtok */
    token = stringtok(str_copy, delimiter);
    while (token != NULL) {
        tokens = realloc(tokens, (count + 1) * sizeof(char *));
        if (!tokens) {
            perror("realloc");
            free(str_copy);
            return (NULL);
        }
        tokens[count] = strdup(token);
        if (!tokens[count]) {
            perror("strdup");
            free(str_copy);
            for (i = 0; i < count; i++) {
                free(tokens[i]);
            }
            free(tokens);
            return (NULL);
        }
        count++;
        token = stringtok(NULL, delimiter);
    }

    free(str_copy);

    /* Add a NULL pointer at the end to indicate the end of the array */
    tokens = realloc(tokens, (count + 1) * sizeof(char *));
    if (!tokens) {
        perror("realloc");
        for (i = 0; i < count; i++) {
            free(tokens[i]);
        }
        return (NULL);
    }
    tokens[count] = NULL;

    return (tokens);
}
