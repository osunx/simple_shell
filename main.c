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
  char *delimtwo = "$$ $?";
  char *cmd = NULL;


   while (1)
   {
       if (argc == 2) {
           execute_commands_from_file(argv[1]);
	   break;
       }
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
	if (containschars(command, delimtwo) != 0) {
	   execute_with_variable_replacement(command);
	} else if (strsearch(command, "exit") != 0) {
            execute_exit(command);
        } else if (stringcmp(command, "env") == 0) {
            execute_env();
        } else if (stringtwocmp(command, "cd", 2) == 0) {
            execute_cd(command);
        } else if (containschars(command, delimone) == 1) {
            execute_separator(command);
        } else if (strstr(command, delim) != NULL) {
            execute_logical_operator(command);
        } else if (strsearch(command, "setenv") != 0 ||
		strsearch(command, "unsetenv") != 0) {
		process_env_command(command);
	}
	else {
            /* Execute the command */
            execute_command(command);
        }
        if (command != NULL) {
           free(command);
        }
   }

 return (EXIT_SUCCESS);
}
