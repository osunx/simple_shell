#include "main.h"

/**
 * getenv_custom - Get the value of an environment variable.
 * @name: The name of the environment variable.
 *
 * Return: The value of the environment variable, or NULL if not found.
 */
char *get_environment(const char *name)
{
size_t name_len;
int i;
char *env_var, *value;
if (name == NULL)
{
return (NULL);
}

name_len = stringlen(name);

for (i = 0; environ[i] != NULL; i++)
{
env_var = environ[i];

if (stringtwocmp(env_var, name, name_len) == 0 && env_var[name_len] == '=')
{
value = stringdup(env_var + name_len + 1);
if (value == NULL)
{
perror("getenv_custom: stringdup");
return (NULL);
}
return (value);
}
}

return (NULL);
}


/**
 * custom_strncmp - Compare two strings up to a given length.
 * @str1: The first string.
 * @str2: The second string.
 * @n: The maximum number of characters to compare.
 *
 * Return: An integer less than, equal to, or greater than zero if
 * str1 is found, respectively, to be less than, to match, or be
 * greater than str2.
 */
int stringtwocmp(const char *str1, const char *str2, size_t n)
{
size_t i;
for (i = 0; i < n; i++)
{
if (str1[i] != str2[i])
{
return str1[i] - str2[i];
}
if (str1[i] == '\0')
{
return (0); /* Reached the end of one of the strings */
}
}
return (0); /* Both strings match up to n characters */
}


/**
 * strinprintf - Format a string and store it in a buffer.
 * @str: Destination buffer for the formatted string.
 * @size: Size of the destination buffer.
 * @format: Format string.
 * @...: Variable arguments to be formatted.
 *
 * Return: The number of characters written (excluding the null byte) on success,
 * or -1 if an error occurred or the formatted string doesn't fit in the buffer.
 */
int strinprintf(char *str, size_t size, const char *format, ...) {
    int n;
    va_list args;

    va_start(args, format);
    n = vsnprintf(str, size, format, args);
    va_end(args);

    if (n < 0 || (size_t)n >= size) {
        /* Handle the case where the formatted string doesn't fit in the buffer. */
        return (-1);
    }

    return (n);
}


/**
 * process_env_command - Process setenv and unsetenv commands.
 * @command: The command string to process.
 *
 * This function processes setenv and unsetenv commands along with their arguments.
 * It calls the respective functions to set or unset the environment variables and
 * handles any errors that may occur during the process.
 */
void process_env_command(char *command) {
    char *name = NULL;
    char *value = NULL;
    int trackunset;

    if (strstr(command, "setenv") == command) {
        /* Process setenv command */
        name = stringtok(command + stringlen("setenv") + 1, " ");
        value = stringtok(NULL, " ");
        if (!name || !value) {
            /* Handle invalid command format */
            write(STDERR_FILENO, "Invalid command format\n", 23);
            return;
        }

        setenv(name, value, 1);
	print_environment("print");
    } else if (strstr(command, "unsetenv") == command) {
        /* Process unsetenv command */
        name = stringtok(command + stringlen("unsetenv") + 1, " ");
        if (!name) {
            /* Handle invalid command format */
            write(STDERR_FILENO, "Invalid command format\n", 23);
            return;
        }
        trackunset = unsetenv(name);
        if (trackunset == -1) {
            /* Handle invalid environment */
            write(STDERR_FILENO, "Invalid environment\n", 20);
        }
    } else {
        /* Handle unknown command */
        write(STDERR_FILENO, "Unknown command\n", 16);
    }

}


/**
 * execute_commands_from_file - Executes the commands from a file.
 * @filename: The name of the file containing the commands.
 *
 * This function opens a file, reads the commands line by line, and executes them.
 * Memory is allocated for each line, and it is freed after execution.
 * The function displays an error message if the file cannot be opened.
 *
 * Return: This function does not return any value.
 */
void execute_commands_from_file(char *filename) {
    FILE *file;  /* File pointer to open the file */
    char *line = NULL;  /* Dynamically allocated buffer to store each line from the file */
    size_t len = 0;  /* Initial length of the line */
    ssize_t read;  /* To store the number of characters read */

    /* Open the file in read mode */
    file = fopen(filename, "r");
    if (file == NULL) {
        write(STDERR_FILENO, "./hsh: 0: Can't open ", 21);
        write(STDERR_FILENO, filename, stringlen(filename));
        write(STDERR_FILENO, "\n", 1);
        exit(EXIT_FAILURE);  /* Terminate the program with an error if the file cannot be opened */
    }

    /* Read each line from the file and execute it*/
    while ((read = getline(&line, &len, file)) != -1) {
        get_system(line);  /* Execute each line from the file using the get_system call */
    }

    if (line) {
        free(line);  /* Free the dynamically allocated memory */
    }

    fclose(file);  /* Close the file after reading and executing the commands */
}

