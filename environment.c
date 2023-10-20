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
 * Return: NULL on success and not needed or return env, NULL on failure.
 */

char **set_environment(char *name, char *value, int overwrite, int mode) {
    /** Declare variables **/
    size_t name_length, value_length, new_var_length, num_vars, j, i;
    char *new_var = NULL;
    char **envarr = NULL;
    int has_existing_environment = 0; /** Flag for existing environment **/

    /** Calculate the lengths of the name and value **/
    name_length = stringlen(name);
    value_length = stringlen(value);
    new_var_length = name_length + value_length + 2;

    /** Find the environment variable **/
    for (i = 0; environ[i] != NULL; i++) {
        if (strncmp(environ[i], name, name_length) == 0 && environ[i][name_length] == '=') {
            has_existing_environment = 1; /** Set the flag for existing environment **/
            /** If the variable exists and overwrite is allowed, modify it **/
            if (overwrite) {
                snprintf(environ[i], new_var_length, "%s=%s", name, value);
                if (strcmp(name, "PATH") == 0) {
                    /** Free the existing PATH variable if it exists **/
                    for (j = 0; environ[j] != NULL; j++) {
                        if (strncmp(environ[j], "PATH=", 5) == 0) {
                            free(environ[j]);
                            break;
                        }
                    }
                }
                if (mode) {
                    /** Print the environment after setting or overwriting **/
                    print_environment("print");
                }
                return (environ); /** Return NULL as environ doesn't need to be freed **/
            } else {
                /** If the variable exists but overwrite is not allowed, return without making changes **/
                return (NULL);
            }
        }
    }

    /** If the variable doesn't exist and overwriting is allowed, create it **/
    new_var = (char *)malloc(new_var_length * sizeof(char));
    if (new_var == NULL) {
        perror("malloc");
        return (NULL); /** Return NULL on failure **/
    }
    snprintf(new_var, new_var_length, "%s=%s", name, value);

    /** Count the number of existing environment variables **/
    num_vars = 0;
    while (environ[num_vars] != NULL) {
        num_vars++;
    }

    /** Allocate memory for the new environment array **/
    envarr = (char **)malloc((num_vars + 2) * sizeof(char *));
    if (envarr == NULL) {
        perror("malloc");
        free(new_var); /** Free the newly allocated variable before returning NULL **/
        return NULL; /** Return NULL on failure **/
    }

    /** Copy existing environment variables to the new array **/
    for (j = 0; j < num_vars; j++) {
        envarr[j] = strdup(environ[j]); /** Use strdup to create a duplicate **/
    }

    /** Add the new environment variable to the end of the array **/
    envarr[num_vars] = new_var;
    envarr[num_vars + 1] = NULL;

    /** If an old environment exists, free it before updating 'environ' **/
    if (has_existing_environment) {
        for (j = 0; environ[j] != NULL; j++) {
            free(environ[j]);
        }
        free(environ);
    }

    if (mode) {
        /** Print the environment after setting or creating a new variable **/
        print_environment("print");
    }
    return (envarr); /** Return the updated environment array that needs to be freed **/
}






/**
* remove_environment - Remove an environment variable.
* @name: The name of the environment variable to remove.
*/
int remove_environment(const char *name) {
    int found = 0; /* 0 if variable not found, 1 if variable found and removed */
    size_t namelength; /* Length of the variable name */
    int i, new_index;
    char *delimiter;

    /* Iterate through the environment variables */
    for (i = 0; environ[i] != NULL; i++) {
        delimiter = strchr(environ[i], '=');
        if (delimiter != NULL) {
            namelength = delimiter - environ[i];
            /* Compare the variable name with the one provided */
            if (strncmp(environ[i], name, namelength) == 0) {
                found = 1;
                free(environ[i]); /* Free the memory for the environment variable */
                environ[i] = NULL; /* Set the pointer to NULL to indicate removal */
            }
        }
    }

    /* If the variable was found, compact the environment to remove NULL entries */
    if (found) {
        new_index = 0;
        for (i = 0; environ[i] != NULL; i++) {
            if (environ[i] != NULL) {
                environ[new_index] = environ[i];
                new_index++;
            }
        }
        environ[new_index] = NULL; /* Null-terminate the environment array */
    }

    return (found);
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
        total_length += stringlen(environ[i]) + 1; /* Add 1 for newline character */
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
