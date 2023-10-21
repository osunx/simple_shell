#include "main.h"

/**
 * is_interactive_mode - Check if the shell is running in interactive mode.
 *
 * Return: 1 if in interactive mode, 0 otherwise.
 */
int isInteractiveMode(void)
{
    return (isatty(STDIN_FILENO) && isatty(STDOUT_FILENO));
}


/**
 * getUserName - Retrieve the username using getpwuid()
 *
 * Return: A pointer to the username string.
 *         NULL on failure.
 */
char *getUserName(void) {
    uid_t uid = getuid();
    struct passwd *pwd = getpwuid(uid);

    if (pwd == NULL) {
        perror("getpwuid");
        exit(EXIT_FAILURE);
    }

    return (pwd->pw_name);
}



/**
 * displayHostName - Display the hostname and working directory
 */
void displayHostName(void) {
    /* Define the prompt string */
    char hostname[1024]; /* Buffer to store the hostname */
    char *pwd = get_environment("PWD"); /* Get the working directory */
    int total_length;
    char *output = NULL;
    int color_code1, color_code2, color_code3;
    /* Calculate the total length of the concatenated string */
    int color_escape_length = 38; /* Length of ANSI escape codes for color */

    /* Check if the shell is running in interactive mode */
    if (isInteractiveMode()) {
        if (gethostname(hostname, sizeof(hostname)) != 0) {
            perror("gethostname");
            exit(EXIT_FAILURE);
        }

        total_length = stringlen(getUserName()) + stringlen(hostname) + stringlen(pwd) + color_escape_length;
        /* ^^ 5 for other characters and null terminator */

        /* Allocate memory for output */
       output = (char *)malloc(total_length * sizeof(char));

        if (output != NULL) {
            color_code1 = rand() % 8 + 30; /* Generates a random color code between 30 and 37 */
            color_code2 = rand() % 8 + 30; /* Generates a random color code between 30 and 37 */
            color_code3 = rand() % 8 + 30; /* Generates a random color code between 30 and 37 */
            /* Format and concatenate the strings with color escape sequences */
            snprintf(output, total_length, "\033[1;%dm%s@%s\033[0m\033[1;%dm:\033[0m\033[1;%dm%s\033[0m$ ", color_code1, getUserName(), hostname, color_code2, color_code3, pwd);
        } else {
            /* Handle memory allocation failure */
            perror("malloc");
            exit(EXIT_FAILURE);
        }
    }

    /* Write the concatenated string to stdout if in interactive mode */
    if (output != NULL) {
        write(STDOUT_FILENO, output, stringlen(output));

        /* Free the allocated memory */
        free(output);
        free(pwd);
    }
}




/**
 * Read input from stdin using getline.
 * @return The input string or NULL if nothing was read.
 */
char *read_command() {
    char *input = NULL;
    size_t len = 0, i;
    ssize_t read;
    /* write(STDOUT_FILENO, "Enter a command: ", 17); */
    read = getline(&input, &len, stdin);
    if (read == -1) {
        free(input);
        if (isInteractiveMode()) {
            handle_errno("EOF");
        }
        return(NULL);
    }

    /* Removal of trailing newline character */
    if (read > 0 && input[read - 1] == '\n') {
        input[read - 1] = '\0';
    }

    /* Remove comments */
    for (i = 0; i < strlen(input); i++) {
        if (input[i] == '#') {
            input[i] = '\0'; /* Replace the '#' with null character to truncate the string */
            break; /* Break from the loop since the comment symbol has been found */
        }
    }
    return (input);
}


/**
 * strsearch - Find the first occurrence of a keyword in a string
 * @str: The string to search
 * @keyword: The keyword to search for
 *
 * Return: Pointer to the first occurrence of the keyword, or NULL if not found
 */
char *strsearch(char *str, const char *keyword) {
    size_t keywordLen = strlen(keyword);
    char nextChar;

    if(str == NULL || keyword == NULL) {
        return (NULL);
    }

    while (*str != '\0') {
        if ((str != NULL || !isalnum((unsigned char)str[-1])) && strncmp(str, keyword, keywordLen) == 0) {
            nextChar = str[keywordLen];
            if (!isalnum((unsigned char)nextChar) && nextChar != '.') {
                return (str);
            }
        }
        str++;
    }

    return (NULL);
}
