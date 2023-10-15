#include "main.h"


/**
 * stringdup - duplicates a string.
 * @str: the string to duplicate
 * rturn: pointer to the duplicated string
 * Return: pointer to the duplicated string
 */
char *stringdup(const char *str)
{
size_t length;
char *duplicate;
if (str == NULL)
return (NULL);

length = stringlen(str);
duplicate = malloc(length + 1);
if (!duplicate)
return (NULL);

stringcpy(duplicate, str);
return (duplicate);
}

/**
 * wputchar - writes the character c to stdout
 * @chars: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */

int wputchar(int chars)
{
static char buffer[BUFFER_SIZE];
int i = 0;

if (chars == '\n' || i >= BUFFER_SIZE - 1)
{
write(1, buffer, i);
i = 0;
if (chars == '\n')
{
/* Write newline if needed */
write(1, "\n", 1);
return (1);
}
}

/* Only print visible ASCII characters, excluding BUF_NULLTERMINATE */
if (chars >= 0 && chars <= 127 && chars != BUF_NULLTERMINATE)
buffer[i++] = chars;

return (1);
}

/**
 * wputs - prints an input string
 * @str: the string or data to be printed
 *
 * Return: Nothing
 */

void wputs(char *str)
{
int i = 0;

if (!str)
return;
while (str[i] != '\0')
{
wputchar(str[i]);
i++;
}
}

/**
 * startwith - checks if prefix is found at the beginning of string
 * @string: string to search
 * @prefix: the prefix to find
 *
 * Return: address of next char after prefix in string or NULL
 */
char *startwith(char *string, char *prefix) {
    int prefix_len = stringlen(prefix);
    int str_len = stringlen(string);

    if (str_len < prefix_len) {
        return (NULL);  /* The string is shorter than the prefix */
    }

    if (stringtwocmp(string, prefix, prefix_len) == 0) {
        return (prefix);  /* Return a pointer to the prefix within the original string */
    }

    return (NULL);  /* Prefix not found */
}

/**
 * strexit - Check if any of the specified
 * characters are present in a string.
 * @string: The input string to be checked.
 * @target: A string containing the characters to be checked for.
 * Return: 1 if any of the characters are found, 0 otherwise.
 */
size_t strexit(char *string, char *target) {
    size_t count = 0;
    size_t stringLen;
    size_t targetLen;
    size_t i, z;

    if (string == NULL || target == NULL) {
        return (-1);
    }

    stringLen = stringlen(string);
    targetLen = stringlen(target);
    for (i = 0; i <= stringLen - targetLen; i++) {
        for (z = 0; z < targetLen; z++) {
            if (string[i + z] != target[z]) {
                break;
            }
        }
        if (z == targetLen) {
            count++;
        }
    }
    return (count);
}
