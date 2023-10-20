#include "main.h"


/* Define the array here */
char **definedalias = NULL;

/**
 * sigint_handler - Handle the SIGINT signal (Ctrl+C).
 * @signum: The signal number.
 */

volatile sig_atomic_t g_signal_received = 0;

void sigint_handler(int signum) {
    if (signum == SIGINT) {
        /* Handle Ctrl+C Process Termination */
        write(STDOUT_FILENO, "trl+C received. Process Terminated...\n", 38);
        g_signal_received = 1;

        /* Display prompt to user in Interactive mode */
        if (isInteractiveMode()) {
            displayHostName();
        }
    }
}

/**
 * main - Entry point of the shell program.
 * @argc: The number of command-line arguments.
 * @argv: An array of command-line arguments.
 *        argv[0] is the name of the program.
 * Return: Always 0.
 */
int main(int argc, char *argv[])
{
  char *command;
  char *delim = "&&||";
  char *delimone = ";";
  char *cmd = NULL;

  /* Ensure the program is called with correct arguments */
  if (argc != 1) {
     write(STDERR_FILENO, "Usage: ", 7);
     write(STDERR_FILENO, argv[0], strlen(argv[0]));
     write(STDERR_FILENO, "\n", 1);
     return( EXIT_FAILURE);
  }

   while (1)
   {
        /* Display user name */
        if (isInteractiveMode()) {
            displayHostName();
        }

        /* Get user input */
        cmd = read_command();

	/* Exit the loop if Ctrl+D (EOF) is encountered */
	if (cmd == NULL) {
	   break;
	}
        command = cmdConstructor(cmd);
        if (strstr(command, "alias") != NULL) {
            free(command);
            return (-1);
        }

        /* Check and execute built-in commands */
        if (stringtwocmp(command, "exit", 4) == 0) {
            execute_exit(command);
        } else if (stringtwocmp(command, "env", 3) == 0) {
            execute_env();
        } else if (stringtwocmp(command, "cd", 2) == 0) {
            execute_cd(command);
        } else if (containschars(command, delimone) == 1) {
            execute_separator(command);
        } else if (containschars(command, delim) == 1) {
            execute_logical_operator(command);
        } else {
            /* Execute the command */
            execute_command(command);
        }
        if (command != NULL) {
           free(command);
        }
   }

 return (EXIT_SUCCESS);
}
