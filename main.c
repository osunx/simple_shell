#include "main.h"


/**
 * sigint_handler - Handle the SIGINT signal (Ctrl+C).
 * @signum: The signal number.
 */

volatile sig_atomic_t g_signal_received = 0;

void sigint_handler(int signum) {
    if (signum == SIGINT) {
        /* Handle Ctrl+C Process Termination */
        write(STDOUT_FILENO, " Ctrl+C received. Process Terminated...\n", 39);
        g_signal_received = 1;
    } else if (signum == SIGTERM) {
        /* Handle Ctrl+D (EOF) or other termination signals */
        write(STDOUT_FILENO, "Ctrl+D (EOF) received. Exiting...\n", 35);
        g_signal_received = 1;
    }
}


/**
 * main - Entry point of the shell program.
 * @argc: The number of command-line arguments.
 * @argv: An array of command-line arguments.
 *        argv[0] is the name of the program.
 * Return: Always 0.
 */

int main(int argc, char *argv[]) {
    char *command;
    char *delim = "&|";
    char *delimone = ";";
    /* For SIGINT and SIGTERM */
    struct sigaction sa;

        /* Ensure the program is called with correct arguments */
    if (argc != 1) {
        write(STDERR_FILENO, "Usage: ", 7);
        write(STDERR_FILENO, argv[0], stringlen(argv[0]));
        write(STDERR_FILENO, "\n", 1);
        return (EXIT_FAILURE);
    }

    /* Set up the SIGINT and SIGTERM handler */
    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL); /* Handle Ctrl+C Process Termination */
    sigaction(SIGTERM, &sa, NULL); /* Handle Ctrl+D (EOF) */

    while (1) {

        /* Check if Ctrl+C or Ctrl+D (EOF) was pressed */
        if (g_signal_received) {
            break; /* Exit the main loop if a signal was received */
        }

        /* Read user input */
        if (isInteractiveMode()) {
            displayHostName();
        }

        /* Get user input */
        command = read_command();

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
            execute_logical_operators(command);
        } else if (stringtwocmp(command, "alias", 5) == 0) {
            execute_alias(command);
        } else {
            /* Execute the command */
            execute_command(command);
        }
        free(command);
    }

    return (EXIT_SUCCESS);
}
