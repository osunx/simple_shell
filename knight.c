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

char **tokenize(char *str, char *delimiter)
{
    char **tokens;
    char *token;
    char *str_copy;
    size_t i, count = 0;

    /* Check if the string is the same as the delimiter */
    if (strcmp(str, delimiter) == 0 || str == NULL) {
        tokens = malloc(sizeof(char *));
        if (!tokens) {
            perror("malloc");
            return (NULL);
        }
        tokens[0] = NULL;  /* If the string is the delimiter itself, return an array with a single NULL pointer */
        return (tokens);
    }

    str_copy = strdup(str); /* Create a copy of the input string */
    if (!str_copy) {
        perror("strdup");
        return (NULL);
    }

    tokens = malloc(sizeof(char *)); /* Allocate memory for at least one pointer */
    if (!tokens) {
        perror("malloc");
        free(str_copy);
        return (NULL);
    }

    /* Tokenize the string using strtok */
    token = strtok(str_copy, delimiter);
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
        token = strtok(NULL, delimiter);
    }

    free(str_copy);
    tokens = realloc(tokens, (count + 1) * sizeof(char *));  /* Allocate space for the final NULL pointer */
    if (!tokens) {
        perror("realloc");
        return NULL;
    }
    tokens[count] = NULL; /* Add the NULL pointer at the end */

    return (tokens);
}


/**
 * strcomdition - Check if a string meets specific conditions
 * @String: The input string to check
 * @stableString: The string containing allowed stable characters
 * @allowedString: The string containing allowed characters other than stable characters
 * @occurence: The number of times stableString is allowed to occur
 * Return: containsNonAllowed if the conditions are met, 0 otherwise
 */

size_t strcondition(char *String, char *stableString, char *allowedString, size_t occurence)
{
    size_t i;
    size_t containsNonAllowed = 0;
    size_t whiteSpace = 0;
    size_t occurenceCount = 0;
    char targetStable[2];
    size_t len;

    if (String == NULL) {
        return (-1);
    }
    if (stableString == NULL) {
        return (-2);
    } 
    if (allowedString == NULL) {
        return (-3);
    }

    len = strlen(stableString);

    if (occurence > 0) {
        for (i = 0; i < len; i++) {
            targetStable[0] = stableString[i];
            targetStable[1] = '\0';

                /* Count occurrences in String */
                occurenceCount = strexit(String, targetStable);

                if (occurenceCount > occurence) {
                    containsNonAllowed = occurenceCount;
                }
        }
    }

    if (containsNonAllowed == 0) {
        i = 0;
        while (String[i] != '\0' && strchr(stableString, String[i])) {
            i++;
        }

        while (String[i] != '\0') {
            if (isspace(String[i])) {
                if (whiteSpace == 0) {
                    whiteSpace++;
                }
            } else if (!strchr(stableString, String[i]) && !strchr(allowedString, String[i])) {
                containsNonAllowed = 1;
            }
            i++;
        }

        if (whiteSpace != 0 && !strexit(allowedString, " ") && !strexit(stableString, " ")) {
            return (whiteSpace);
        }

        return (containsNonAllowed);
    }

    return (containsNonAllowed);
}


/**
 * containschars - Check if a character is present in a string.
 * @string: The string to search in.
 * @target: The character to search for.
 *
 * Return: 1 if the character is found, 0 otherwise.
 */

int containschars(const char *string, char *target)
{
    while (*string)
    {
        if (*string == *target)
        {
            return 1; /* Found the character in the string */
        }
        string++; /* Move to the next character in the string */
    }
    return 0; /* Character not found in the string */
}
