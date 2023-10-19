#include "main.h"


/**
 * charsBeforeWord - Counts the characters before a specified word in a string
 * @string: The input string
 * @word: The word to search for in the string
 *
 * Return: The number of characters before the specified word, -1 if the word is not found
 */

size_t charsBeforeWord(char *string, char *word) {
    size_t length;
    size_t wordlen;
    size_t i, j;
    size_t found = 0;

    if (string == NULL) {
        return (-2);
    }

    length = stringlen(string);
    wordlen = strlen(word);

    for (i = 0; i <= length - wordlen; i++) {
        if (string[i] == word[0]) {
            found = 1;
            for (j = 0; j < wordlen; j++) {
                if (string[i + j] != word[j]) {
                    found = 0;
                    break;
                }
            }
            if (found) {
                return (i);
            }
        }
    }
    return (-1); /** Specified word not found **/
}


/**
 * extractAliases - Extracts aliases from a string based on a keyword
 * @string: The string from which aliases are to be extracted
 * @keyword: The keyword to search for extracting aliases
 *
 * Return: A double pointer to the extracted aliases
 */
char **extractSubstrings(char *string, const char *keyword) {
    size_t count = 0;
    size_t length;
    char *nextToken;
    char *str = stringdup(string);
    char **aliases;
    char *token;

    /* Check if the string or keyword is NULL */
    if (string == NULL || keyword == NULL) {
        return (NULL);
    }

    token = strsearch(str, keyword);
    if (token == NULL) {
	free(str);
        return (NULL);
    }

    /* Allocate memory for the aliases array */
    aliases = (char **)malloc(sizeof(char *));
    if (!aliases) {
        perror("malloc");
        return (NULL);
    }

    /* Loop through the string to extract aliases based on the keyword */
    while (token != NULL) {
        nextToken = strsearch(token + 1, keyword);
        if (nextToken) {
            aliases = (char **)realloc(aliases, (count + 1) * sizeof(char *));
            if (!aliases) {
                perror("realloc");
                return (NULL);
            }

            /* Extract the substring between the two occurrences of the keyword */
            length = nextToken - token;
            aliases[count] = (char *)malloc((length + 1) * sizeof(char));
            strncpy(aliases[count], token, length);
            aliases[count][length] = '\0';
            count++;
        } else {
            nextToken = str + strlen(str);
            length = nextToken - token;
            aliases = (char **)realloc(aliases, (count + 1) * sizeof(char *));
            aliases[count] = (char *)malloc((length + 1) * sizeof(char));
            strncpy(aliases[count], token, length);
            aliases[count][length] = '\0';
            count++;
            break;
        }

        token = nextToken;
    }

    /* Set the last element of the aliases array to NULL */
    aliases = (char **)realloc(aliases, (count + 1) * sizeof(char *));
    aliases[count] = NULL;

    /* Free the memory allocated for the duplicated string */
    free(str);

    /* Return the array of extracted aliases */
    return (aliases);
}


/**
 * addAlias - Add or update aliases in the definedalias array
 * @strings: The string containing the aliases to be added or updated
 */
void addAlias(char *strings) {
    char *aliasDelimiter = "alias ";
    size_t currentSize = 0;
    char *alias;
    size_t aliasAt, aliasPosition, i, j, k, o = -1;
    char *firstQuote, *endQuote, *trimmedAlias, *found;
    char *aliasPointer;
    size_t tempAliasLen, foundIndex;
    char *tempFound, *temp;
    char **aliases = NULL;
    char **tempDefinedAlias = NULL;

    /* Check if the input string is NULL */
    if (strings == NULL || strcmp(strings, "alias") == 0) {
        return;
    }

     /* Extract aliases from the input string */
    aliases = extractSubstrings(strings, "alias");
    if (aliases == NULL) {
        return;
    }

    /* Allocate memory for the temporary definedalias array */
    tempDefinedAlias = (char **)malloc(sizeof(char *));

    if (!tempDefinedAlias) {
        perror("malloc");
        return;
    }

    /* Copy the existing definedalias array to the temporary array */
    for (k = 0; definedalias && definedalias[k] != NULL; k++) {
        tempDefinedAlias = (char **)realloc(tempDefinedAlias, (currentSize + 2) * sizeof(char *));
        if (!tempDefinedAlias) {
            perror("realloc");
            return;
        }
        tempDefinedAlias[currentSize] = strdup(definedalias[k]);
        tempDefinedAlias[currentSize + 1] = NULL;
        currentSize++;
    }

    /* Iterate through the extracted aliases and update the definedalias array */
    for (k = 0; aliases[k] != NULL; k++) {
        alias = strdup(aliases[k]);
        aliasPointer = strstr(alias, aliasDelimiter);

        if (alias == NULL) {
            continue;
        }

        /* Process the alias and its value within the single quotes */
        while (aliasPointer != NULL) {
            aliasAt = aliasPointer - alias;
            aliasPosition = aliasAt + strlen(aliasDelimiter);
            i = 0;

            /** Find the second occurrence of the single quote **/
            firstQuote = strstr(aliasPointer, "'");
            if (firstQuote == NULL) {
                break;
            }
            endQuote = strstr(firstQuote + 1, "'");
            if (endQuote == NULL) {
                break;
            }

            /** Extract the alias and its value within the single quotes **/
            trimmedAlias = (char *)malloc((endQuote - aliasPointer) * sizeof(char));
            if (trimmedAlias == NULL) {
                perror("malloc");
                return;
            }

            /** Copy non-' characters to trimmedAlias, skipping the first single quote **/
            while ((size_t)(aliasPosition) < (size_t)(endQuote - alias)) {
                if (alias[aliasPosition] != '\'') {
                    trimmedAlias[i] = alias[aliasPosition];
                    i++;
                }
                aliasPosition++;
            }
            trimmedAlias[i] = '\0';

            /** Find the position of the '=' character **/
            found = strstr(trimmedAlias, "=");
            if (found != NULL) {
                foundIndex = -1;
                for (j = 0; j < currentSize; j++) {
                    temp = strdup(tempDefinedAlias[j]);
                    tempFound = strstr(temp, "=");
                    if (tempFound != NULL) {
                        tempAliasLen = tempFound - temp;
                        if (strncmp(temp, trimmedAlias, tempAliasLen) == 0) {
                            foundIndex = j;
                            break;
                        }
                    }
                    free(temp);
                }

                if (foundIndex != o) {
                    /** Overwrite the alias if it already exists at that index **/
                    free(tempDefinedAlias[foundIndex]);
                    tempDefinedAlias[foundIndex] = strdup(trimmedAlias);
                } else {
                    /** Reallocate memory to accommodate the new alias command **/
                    tempDefinedAlias = (char **)realloc(tempDefinedAlias, (currentSize + 2) * sizeof(char *));
                    tempDefinedAlias[currentSize] = strdup(trimmedAlias);
                    tempDefinedAlias[currentSize + 1] = NULL;
                    currentSize++;
                }
            }

            free(trimmedAlias); /** Free the memory for trimmedAlias **/
            alias = endQuote + 1;
            aliasPointer = strstr(alias, aliasDelimiter);
        }
        alias = NULL;
    }

    /** Free the memory allocated for the old definedalias array **/
    if (definedalias != NULL) {
        for (i = 0; definedalias[i] != NULL; i++) {
            free(definedalias[i]);
        }
        free(definedalias);
    }

    /** Update the definedalias array with the new changes **/
    definedalias = tempDefinedAlias;

    /** Free memory allocated for the aliases array **/
    for (i = 0; aliases[i] != NULL; i++) {
        free(aliases[i]);
    }
    free(aliases);
}






/**
* This function 'check_alias' is used to check if the given command contains any defined aliases.
* It reconstructs the command by replacing any aliases found within it with their corresponding values.
* It returns the modified command with aliases replaced, or NULL if there is an error or no aliases are defined.
**/
char *checkAlias(char *command) {
    /** Initialize variables for reconstructing
      * the command and processing the aliases **/
    char *reconstructedCommand;
    char *found;
    size_t aliasLen, wordStart, newCommandLen;
    size_t i = 0;
    char *alias, *word, *aliasValue, *newCommand;

    /** Check if any aliases are defined **/
    if (definedalias == NULL) {
        return (NULL);
    }

    /** Allocate memory for the reconstructed command **/
    reconstructedCommand = (char *)malloc(strlen(command) + 1);
    if (reconstructedCommand == NULL) {
        perror("malloc");
        return (NULL);
    }

    /** Copy the original command to the reconstructed one **/
    strcpy(reconstructedCommand, command);

    /** Iterate through defined aliases to find and replace them in the command **/
    while (definedalias[i] != NULL) {

        /** Find the position of '=' in the alias definition **/
        found = strstr(definedalias[i], "=");
        if (found != NULL) {
            /** Calculate the length of the alias **/
            aliasLen = found - definedalias[i];

            /** Allocate memory for the alias **/
            alias = (char *)malloc((aliasLen + 1) * sizeof(char));
            if (alias == NULL) {
                perror("malloc");
                return (NULL);
            }

            /** Copy the alias from the definition **/
            strncpy(alias, definedalias[i], aliasLen);
            alias[aliasLen] = '\0';

            /** Find and replace the alias in the reconstructed command **/
            word = reconstructedCommand;
            while ((word = strstr(word, alias)) != NULL) {
                wordStart = word - reconstructedCommand;

                /** Check if the found alias is surrounded by spaces or at the start/end of the command **/
                if ((wordStart == 0 || reconstructedCommand[wordStart - 1] == ' ') &&
                    (reconstructedCommand[wordStart + aliasLen] == ' ' || reconstructedCommand[wordStart + aliasLen] == '\0')) {

                    /** Replace the alias with its corresponding value **/
                    aliasValue = found + 1;
                    newCommandLen = strlen(reconstructedCommand) - aliasLen + strlen(aliasValue) + 1;
                    newCommand = (char *)malloc(newCommandLen * sizeof(char));
                    if (newCommand == NULL) {
                        perror("malloc");
                        return (NULL);
                    }

                    /** Copy the part of the command before the alias, the alias value, and the part after the alias **/
                    strncpy(newCommand, reconstructedCommand, wordStart);
                    strcpy(&newCommand[wordStart], aliasValue);
                    strcpy(&newCommand[wordStart + strlen(aliasValue)], &reconstructedCommand[wordStart + aliasLen]);

                    /** Free the old reconstructed command and assign the new one **/
                    free(reconstructedCommand);
                    reconstructedCommand = newCommand;

                    /** Move the word pointer to the end of the replaced alias **/
                    word = &reconstructedCommand[wordStart + strlen(aliasValue)];
                } else {

                    /** If the alias is not surrounded by spaces, move the word pointer to the next character **/
                    word++;
                }
            }

            /** Free the memory allocated for the alias **/
            free(alias);
        }

        /** Move to the next defined alias **/
        i++;
    }

    /** Return the reconstructed command with aliases replaced **/
    return (reconstructedCommand);
}

/**
 * cmdConstructor - Constructs a command based on the given input and predefined aliases.
 * @command: The input command to be processed.
 *
 * Return: Returns the processed command or the original command if no changes were made.
 */
char *cmdConstructor(char *command) {
    char *reconstructedCommand = NULL;
    size_t i;
    char *aliasMessage = "An alias is a way to customize and create shortcuts for commands.\n"
                    "To create an alias, you can use the following syntax:\n"
                    "alias <custom_command_name>='<actual_command>'\n"
                    "Here are some examples of aliases:\n"
                    "alias l=ls -CF\n"
                    "alias ll=ls -alF\n"
                    "alias la=ls -A\n"
                    "alias c=clear\n"
                    "alias ..=cd ..\n"
                    "alias gs=git status\n"
                    "This create an alias for the current session only, you can use \"alias last='history | tail -n 1'\"\n"
                    "to create an alias named 'last' that displays the last executed command.\n";


    /* Check if */
    if (definedalias != NULL && strcmp(command, "alias") == 0) {
        for (i = 0; definedalias[i] != NULL; i++) {
            write(STDOUT_FILENO, "alias ", 6);
            write(STDOUT_FILENO, definedalias[i], strlen(definedalias[i]));
            write(STDOUT_FILENO, "\n", 1);
        }
    } else if (definedalias == NULL && strcmp(command, "alias") == 0) {
               write(STDOUT_FILENO, aliasMessage, strlen(aliasMessage));
    }

    if (strsearch(command, "alias") != NULL) {
        addAlias(command);
    }
    if (strsearch(command, "alias") == NULL) {
        reconstructedCommand = checkAlias(command);
    }

    if (reconstructedCommand != NULL) {
        if (strcmp(reconstructedCommand, command) != 0) {
            return (reconstructedCommand);
        }
    } else {
        free(reconstructedCommand);
        return (command);
    }

    /* Return command by default if no other conditions are met */
    return (command);
}
