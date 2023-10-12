#include "main.h"

/**
 * create_environment - Create a new environment array.
 *
 * Return: A pointer to the newly created environment array.
 */

char **create_environment(void)
{
char **envp;
int env_count = 0;
int i, j;

/* Count the number of environment variables */
while (environ[env_count] != NULL)
{
env_count++;
}

/* Allocate memory for the environment variable array */
envp = malloc((env_count + 1) * sizeof(char *));
if (envp == NULL)
{
perror("malloc");
exit(EXIT_FAILURE);
}

/* Copy each environment variable to the array */
for (i = 0; i < env_count; i++)
{
envp[i] = stringdup(environ[i]);
if (envp[i] == NULL)
{
perror("strdup");
/* Free previously allocated memory */
for (j = 0; j < i; j++)
{
free(envp[j]);
}
free(envp);
exit(EXIT_FAILURE);
}
}

envp[env_count] = NULL; /* NULL-terminate the array */

return (envp);
}


/**
 * set_environment - Initialize a new environment
 * variable or modify an existing one.
 *
 * @name: The name of the environment variable.
 * @value: The value to set for the environment variable.
 * @overwrite: Flag indicating whether to overwrite an existing variable.
 * @mode: Flag indicating whether to print the environment after setting.
 *
 * Return: 0 on success, -1 on failure.
 */
int set_environment(const char *name, const char *value, int overwrite, int mode) {

    /** Declare variables for new environment variable **/
    char *new_var = NULL;
    int i;
    size_t num_vars = 0;
    char **new_environ = NULL;
    size_t j;

    /** Calculate the lengths of the name and value **/
    size_t name_length = strlen(name);
    size_t value_length = strlen(value);

    /** Find the environment variable **/
    for (i = 0; environ[i] != NULL; i++) {
        if (strncmp(environ[i], name, name_length) == 0 && environ[i][name_length] == '=') {
            if (overwrite) {
                /** Modify the existing environment variable **/
                new_var = (char *)malloc(name_length + value_length + 2);
                if (new_var == NULL) {
                    perror("malloc");
                    return -1; /** Return -1 on failure **/
                }
                sprintf(new_var, "%s=%s", name, value);
                environ[i] = new_var; /** Update the environment variable **/

                if (mode) {
                    /** Print the environment after setting or overwriting **/
                    print_environment("print");
                }

                return (0); /** Return 0 on success **/
            } else {
                return (0); /** Environment variable exists but not allowed to overwrite **/
            }
        }
    }

    /** If the variable doesn't exist and overwriting is allowed, create it **/
    new_var = (char *)malloc(name_length + value_length + 2);
    if (new_var == NULL) {
        perror("malloc");
        return (-1); /** Return -1 on failure **/
    }
    sprintf(new_var, "%s=%s", name, value);

    /** Count the number of existing environment variables **/
    while (environ[num_vars] != NULL) {
        num_vars++;
    }

    /** Allocate memory for the new environment array **/
    new_environ = (char **)malloc((num_vars + 2) * sizeof(char *));
    if (new_environ == NULL) {
        perror("malloc");
        free(new_var); /** Free the newly allocated variable before returning -1 **/
        return (-1); /** Return -1 on failure **/
    }

    /** Copy existing environment variables to the new array **/
    for (j = 0; j < num_vars; j++) {
        new_environ[j] = environ[j];
    }

    /** Add the new environment variable to the end of the array **/
    new_environ[num_vars] = new_var;
    new_environ[num_vars + 1] = NULL;

    /** Update the 'environ' pointer to point to the new array **/
    environ = new_environ;

    if (mode) {
        /** Print the environment after setting or creating a new variable **/
        print_environment("print");
    }

    return (0); /** Return 0 on success **/
}




/**
* remove_environment - Remove an environment variable.
* @name: The name of the environment variable to remove.
*/

void remove_environment(const char *name)
{
char **new_env = NULL;
int new_env_size = 0;
char *delimiter;
char delim = '=';
int namelength;
int i;
int j;

/* Iterate through the environment variables */
for (i = 0; environ[i] != NULL; i++)
{
delimiter = strchr(environ[i], delim);
if (delimiter != NULL)
{
/* Calculate the length of the variable  */
namelength = delimiter - environ[i];

/* Compare the variable name with the one provided*/
if (strncmp(environ[i], name, namelength) != 0)
{
/* Copy the variable to the new environment array */
new_env_size++;
new_env = realloc(new_env, sizeof(char *) * (new_env_size + 1));
if (new_env == NULL)
{
perror("realloc");
exit(EXIT_FAILURE);
}
new_env[new_env_size - 1] = strdup(environ[i]);
}
}
}

/* Null-terminate the new environment array */
new_env[new_env_size] = NULL;

/* Free the old environment array */
for (j = 0; environ[j] != NULL; j++)
{
free(environ[j]);
}
free(environ);

/* Set environ to the new environment array */
environ = new_env;
}
/**
* print_environment - Print the environment variables.
* @mode : write the environment or return it.
* Return: A pointer to a string containing the environment variables.
*/

char *print_environment(char *mode) {
    int i = 0;
    size_t total_length = 0;
    char *env_string;
    char *current_position;

    /* Calculate the total length of the environment variables */
    while (environ[i] != NULL) {
        total_length += strlen(environ[i]) + 1; /* Add 1 for newline character */
        i++;
    }

    /* Allocate memory for the string + 1 space for null-terminator */
    env_string = (char *)malloc(total_length + 1);
    if (env_string == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    /* Copy the environment variables into the string */
    i = 0;
    current_position = env_string;
    while (environ[i] != NULL) {
        strcpy(current_position, environ[i]);
        strcat(current_position, "\n");
        current_position += strlen(environ[i]) + 1;
        i++;
    }
    *current_position = '\0';

    if (strcmp(mode, "print") == 0 || strcmp(mode, "write") == 0) {
        /* Write the environment to stdout */
        write(STDOUT_FILENO, env_string, strlen(env_string));
        free(env_string); /* Free the allocated memory */
        return (NULL);      /* Return NULL when in "print" mode */
    } else if (strcmp(mode, "none") == 0 || strcmp(mode, "return") == 0) {
        /* Return the environment as a character string */
        return env_string;
    } else {
        /* Invalid mode: Write the error message to stdout */
        write(STDOUT_FILENO, "Invalid mode: ", 14);
        write(STDOUT_FILENO, mode, strlen(mode));
        write(STDOUT_FILENO, "\n", 1);
        free(env_string); /* Free the allocated memory */
        return (NULL);
    }
}


/**
 * free_environment - Frees memory allocated for an environment array.
 * @env_array: The environment array to be freed.
 *
 * This function frees the memory allocated for each string in the environment
 * array and then frees the memory of the environment array itself.
 */
void free_environment(char **env_array)
{
int i;
if (env_array == NULL)
return;

for (i = 0; env_array[i] != NULL; i++)
{
free(env_array[i]);
}

free(env_array);
}

/* Print the environment variables */
/* void print_environment(void) */
/* { */
/*     int i = 0; */
/*    while (environ[i] != NULL) */
/*    { */
/*         printf("%s\n", environ[i]); */
/*         i++; */
/*     } */
/* } */
