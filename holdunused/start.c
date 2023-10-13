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
    char prompt[] = "$ ";
    char hostname[1024]; /* Buffer to store the hostname */
    char *pwd = get_environment("PWD"); /* Get the working directory */
    int total_length;
    char *output = NULL;
    /* Calculate the total length of the concatenated string */
    int color_escape_length = 11; /* Length of ANSI escape codes for green color */

    /* Check if the shell is running in interactive mode */
    if (isInteractiveMode()) {
        if (gethostname(hostname, sizeof(hostname)) != 0) {
            perror("gethostname");
            exit(EXIT_FAILURE);
        }

        total_length = stringlen(getUserName()) + stringlen(hostname) + stringlen(pwd) + color_escape_length + 5;
         /* ^^ 3 for other characters and null terminator */

        /* Allocate memory for output */
        output = (char *)malloc(total_length);

        if (output != NULL) {
            /* Format and concatenate the strings with color escape sequences */ /* ANSI escape codes for green color */
            strinprintf(output, total_length, "\033[1;32m%s@%s:%s\033[0m# ", getUserName(), hostname, pwd);
        } else {
            /* Handle memory allocation failure */
            perror("malloc");
            exit(EXIT_FAILURE);
        }
    }

    /* Write the concatenated string to stdout if in interactive mode */
    if (output != NULL) {
        write(0, output, stringlen(output));

        /* Free the allocated memory */
        if (output != NULL) {
	    free(output);
	}
	if (pwd != NULL) {
	    free(pwd);
	}
	
    } else {
        /* Write dollar sign and space if not in interactive mode */
        write(STDIN_FILENO, prompt, stringlen(prompt));
    }
}



/**
 * Read input from stdin using getline.
 * @return The input string or NULL if nothing was read.
 */
char *read_command()
{
char *input = NULL;
size_t len = 0;
ssize_t read;
  /* write(STDOUT_FILENO, "Enter a command: ", 17); */
  read = getline(&input, &len, stdin);
  if (read == -1)
  {
  free(input);
  return (NULL);
  }

  /*Removal of trailing newline character*/
  if (read > 0 && input[read - 1] == '\n')
  {
   input[read - 1] = '\0';

  }
return (input);
}
