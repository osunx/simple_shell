#include "main.h"

/**
 * stringcpy - is a clone of strcpy function Copy a string.
 * @dest: Destination buffer
 * @src: Source string
 *
 * This function copies the string from @src to @dest and returns
 * a pointer to the original destination buffer @dest.
 *
 * Return: A pointer to the original @dest buffer.
 */

char *stringcpy(char *dest, const char *src)
{
	/*Store the original destination pointer*/
	char *origindest = dest;
	/*Check for null pointers*/
	if (dest == NULL || src == NULL)
	{
		return (dest);
	}

	/*Avoid copying from source to itself*/
	if (dest == src)
	{
		return (dest);
	}

	while (*src != '\0')
	{
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0'; /* Null-terminate the destination string */
	return (origindest); /* Return the original destination pointer */
}

/**
* stringlen - Calculate the length of a string
* @str: The input string
* Return: The length of the string
*/

int stringlen(const char *str)
{
int len = 0;
	while (str[len] != '\0')
	{
	len++;
	}
	return (len); /* Return the length of the string */
}


/**
 * stringcmp - Cunstomized string comparison, Compare two strings
 * @s1: First string
 * @s2: Second string
 * Return: Negative if s1 < s2, positive if s1 > s2, zero if s1 == s2
 */
int stringcmp(const char *s1, const char *s2)
{
	while (*s1 && *s2 && *s1 == *s2)
	{
		s1++;
		s2++;
	}

	if (*s1 == *s2)
	{
		return (0);
	}
	else
	{
		return ((*s1 < *s2) ? -1 : 1);
	}
}

/**
 * stringcat - Concatenate two strings
 * @dest: Destination buffer
 * @src: Source string
 * Return: Pointer to the destination buffer
 */
char *stringcat(char *dest, const char *src)
{
char *ret = dest; /*Store starting address of dest*/

	/*Move to the end of dest*/
	while (*dest)
		{
	dest++;
	}

	/*Concatenate src to dest*/
	while (*src)
		{
	*dest = *src;
	dest++;
	src++;
	}

	*dest = '\0'; /*Null-terminate the concatenated string*/
	return (ret); /*Return starting address of dest*/
}

/**
 * stringtok - Custom implementation of strtok using stringsplit.
 *
 * @str: The input string to tokenize.
 * @delim: The delimiter string.
 *
 * Return: A pointer to the next token, or NULL if no more tokens are found.
 */

char *stringtok(char *str, char *delim)
{
  char *tokens = NULL;

  /* Tokenize the input string */
  tokens = strtok(str, delim);

 return (tokens);
}
