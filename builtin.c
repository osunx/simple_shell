#include "main.h"

/**
 * execute_exit - Execute the built-in exit command.
 * plus arguments for the built-in exit
 * exit status, where status is an integer used to
 * exit the shell.
 *
 * @input: The input command.
 */
void execute_exit(char *input) {
char *trimmed_input = input;
size_t input_length = 0;
int status = 0;

/* Trim leading and trailing spaces */
while (*trimmed_input == ' ' || *trimmed_input == '\t') {
trimmed_input++;
}

input_length = strlen(trimmed_input);

while (input_length > 0 && (trimmed_input[input_length - 1] == ' ' || trimmed_input[input_length - 1] == '\t')) {
input_length--;
}

trimmed_input[input_length] = '\0';

/* Check if the trimmed input is "exit" */
if (strcmp(trimmed_input, "exit") == 0) {
free(input);
exit(0); /* Exit the shell with status 0 */
} else if (strncmp(trimmed_input, "exit ", 5) == 0) {
/* Check if the trimmed input starts with "exit " */

/* If yes, extract the status and exit with that status */
if (is_integer(trimmed_input + 5)) {
status = atoi(trimmed_input + 5);
if (status >= 0) {
free(input);
exit(status);
} else {
write(STDERR_FILENO, "./hsh: 1: exit: Illegal number: ", 32);
write(STDERR_FILENO, trimmed_input + 5, strlen(trimmed_input + 5));
write(STDERR_FILENO, "\n", 1);
free(input);
exit(2); /* Exit with status 2 for an invalid exit command */
}
}
} else {
/* The input is not a valid exit command */
write(STDERR_FILENO, "Invalid command: ", 17);
write(STDERR_FILENO, trimmed_input, strlen(trimmed_input));
write(STDERR_FILENO, "\n", 1);
free(input);
exit(2); /* Exit with status 2 for an invalid command */
}
}


/**
 * execute_env - Execute the built-in env command.
 */
void execute_env()
{
/* Count the number of environment variables */
size_t env_count = 0;
char **env;
char **sorted_env;
size_t i;
for (env = environ; *env != NULL; env++)
{
env_count++;
}

/* Create an array to store environment variables */
sorted_env = malloc((env_count + 1) * sizeof(char *));
if (sorted_env == NULL)
{
perror("malloc");
exit(EXIT_FAILURE);
}

/* Copy environment variables to the array */
for (i = 0; i < env_count; i++)
{
sorted_env[i] = environ[i];
}
sorted_env[env_count] = NULL;

/* Sort the environment variables */
/*qsort(sorted_env, env_count, sizeof(char *), compare_strings);*/

/* Print the sorted environment variables */
for (i = 0; i < env_count; i++)
{
write(STDOUT_FILENO, sorted_env[i], stringlen(sorted_env[i]));
write(STDOUT_FILENO, "\n", 1);
}

/* Free allocated memory */
free(sorted_env);
}



/**
* execute_separator - Execute commands separated by ';'.
*
* @input: The input commands.
*/
void execute_separator(char *input)
{
char *command = stringtok(input, ";\n");
while (command != NULL)
{
get_system(command);
command = stringtok(NULL, ";\n");
}
}


/**
* execute_logical_operators - Execute commands with logical operators.
*
* @command: The input commands.
* Function to execute commands based on logical operators && and ||
**/
void execute_logical_operator(char *command) {
    char *token;
    char *delimiter = "&&";
    int result = 1;
    char *trimmed_token ;
    char *end;
    int execution_status;
    char *or_token;
    char *second_command;
    int or_execution_status;

    /* Tokenize the command based on delimiters && */
    token = stringtok(command, delimiter);

    while (token != NULL) {
        /* Remove leading and trailing white spaces */
        trimmed_token = token;
        while (*trimmed_token == ' ') {
            ++trimmed_token;
        }

        end = trimmed_token + strlen(trimmed_token) - 1;
        while (end > trimmed_token && *end == ' ') {
            *end-- = '\0';
        }

        execution_status = get_system(trimmed_token);
        if (execution_status == -1) {
            if (isInteractiveMode() && errno == ENOENT) {
                free(command);
                exit(2);
            }
        } else if (execution_status != 0) {
            result = 0;
        }

        token = stringtok(NULL, delimiter);
    }

    if (!result) {
        /* If any command failed, execute the second part after || */
        or_token = strstr(command, "||");
        if (or_token != NULL) {
            second_command = or_token + 2; /* Skip the || */
            /* Remove leading and trailing white spaces */
            while (*second_command == ' ') {
                ++second_command;
            }
            or_execution_status = get_system(second_command);
            if (or_execution_status == -1) {
                if (isInteractiveMode() && errno == ENOENT) {
                    free(command);
                    exit(2);
                }
            }
        }
    }
}
