#include "main.h"


#define ERROR_MESSAGE_SIZE 1024

/**
 * get_command_path - Get the full path of a command using PATH.
 * @command: The command to find.
 *
 * Return: The full path of the command if found, or the original command if not found.
 */
char *get_command_path(char *command) {
    /* Get the PATH environment variable */
    char *path = get_environment("PATH");
    /* Duplicate the PATH variable for tokenization */
    char *path_env = stringdup(path);
    /* Tokenize the PATH variable to search for the command */
    char *path_token = stringtok(path_env, ":");
    char *full_path = NULL;
    char error_message[ERROR_MESSAGE_SIZE];

    /* Loop through each directory in the PATH variable */
    while (path_token != NULL) {
        /* Allocate memory for the full path */
        full_path = (char *)malloc(stringlen(path_token) + stringlen(command) + 2);

        if (full_path == NULL) {
            perror("malloc");
            free(path_env);
            free(path);
            exit(1);  /* Handle memory allocation failure by exiting */
        }

        /* Construct the full path by concatenating directory and command */
        stringcpy(full_path, path_token);
        stringcat(full_path, "/");
        stringcat(full_path, command);

        /* Check if the constructed path is executable */
        if (access(full_path, X_OK) == 0) {
            free(path_env);
            free(path);
            return (full_path);  /* Return the valid path */
        }

        free(full_path);
        full_path = NULL;

        /* Get the next directory in the PATH */
        path_token = stringtok(NULL, ":");
    }

    /* Free allocated memory for path_env and path */
    free(path_env);
    free(path);

    /* If no valid path is found, return NULL and print an error message */
    snprintf(error_message, ERROR_MESSAGE_SIZE, "./hsh: 1: %s: not found\n", command);
    free(command);
    /* Write the error message to STDERR_FILENO */
    write(STDERR_FILENO, error_message, stringlen(error_message));

    if (!isInteractiveMode()) {
        exit(127);
       }

    return (NULL);
}



/*** execute_command - Execute a command using fork and execve.
 * @command: The command to execute.
 *
 * Return: 0 on success, -1 on failure.
 ***/

int execute_command(char *command) {
    /*** Both modes ***/
    pid_t child_pid;
    int status;
    char *full_path;
    char **modified_env;
    char arrindex[2048];

    /*** Tokenization ***/
    char *delim = " ";
    char **args = NULL;

    /*** Pipeline mode ***/
    char *pipe_delim = " \t\n";
    char **argv = NULL;
    int argc = 0;
    int pipe_fd[2];

    /*** Tokenize the current command based on mode ***/
    if  (isInteractiveMode()) {
        args = tokenize(command, delim);
	if (args[0] == NULL) {
	   /*** Return -1 if tokenization fails ***/
	   free_environment(args);
	   return (-1);
	}
    }
    else {
        argv = tokenize(command, pipe_delim);
	if (argv[0] == NULL) {
	   free_environment(argv);
	   /*** Return -1 if tokenization fails ***/
	   return (-1);
	}
    }

    /*** Get the full path of the command if not provided with a path ***/
    if (strexit(isInteractiveMode() ? args[0] : argv[0], "/") == 0) {

        /***Get path based on mode***/
        if (isInteractiveMode()) {
            full_path = get_command_path(args[0]);
        } else {
            full_path = get_command_path(argv[0]);
        }

        if (full_path == NULL) {
            free(args);
            return (-1);
        } else {
            if (isInteractiveMode()) {
                free(args[0]);
                args[0] = full_path;
            } else {
                free(argv[0]);
                argv[0] = full_path;
            }
        }
    }

    /*** Create and pass the modified environment ***/
    modified_env = create_environment();
    if (modified_env == NULL) {
        if (isInteractiveMode()) {
            free_environment(args);
        } else {
            if (argv != NULL) {
                free_environment(argv);
            }
        }
        exit(127);
    }

    if (!isInteractiveMode() && argv != NULL) {
        /*** Count the number of commands in the pipeline ***/
        while (argv[argc] != NULL) {
            argc++;
        }
    }


    child_pid = fork();
    if (child_pid == 0) {
        /*** Child process ***/
        if (isInteractiveMode()) {
            /*** Interactive mode: Execute the command directly ***/

            if (execve(args[0], args, modified_env) == -1) {
		/*** Copy args[0] and free args array ***/
		stringcpy(arrindex, args[0]);
		free(command);
		free_environment(modified_env);
		free_environment(args);
                /*** Handle execve failure ***/
               handle_errno(arrindex);
            }
        } else {
            /*** Pipeline mode: Set up pipes and execute the command ***/
            if (argc > 0) {
                /*** Create a pipe ***/
                if (pipe(pipe_fd) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }

                /*** Set up pipes and execute the command ***/
                if (argc > 0) {
                    /*** Connect the input of this command to the output of the previous command ***/
                    dup2(pipe_fd[0], STDIN_FILENO);
                }
                if (argv[argc + 1] != NULL) {
                    /*** Connect the output of this command to the input of the next command ***/
                    dup2(pipe_fd[1], STDOUT_FILENO);
                }

                /*** Close the unused pipe ends ***/
                close(pipe_fd[0]);
                close(pipe_fd[1]);
            }

            if (execve(argv[0], argv, modified_env) == -1) {
		/*** Copy argv[0] and free argv array ***/
		stringcpy(arrindex, argv[0]);
		free(command);
		free_environment(modified_env);
		free_environment(argv);
                /*** Handle execve failure ***/
                handle_errno(arrindex);
            }
        }
    } else if (child_pid > 0) {
        /*** Parent process ***/
        if (!isInteractiveMode()) {
            if (argc > 0) {
                /*** Close the input end of the pipe in pipeline mode ***/
                close(pipe_fd[0]);
            }
            if (argv[argc + 1] != NULL) {
                /*** Close the output end of the pipe in pipeline mode ***/
                close(pipe_fd[1]);
            }
        }
        waitpid(child_pid, &status, 0);

        /*** Placeholder if more waitpid functionality is needed ***/
    }

    /*** Free the memory for arguments and environment ***/
    if (isInteractiveMode()) {
	if (args != NULL) {
	    free_environment(args);
	 }

    } else {
        if (argv != NULL) {
            free_environment(argv);
        }

    }
    free_environment(modified_env);
    /*printf("argv[0] is: %s\nargv is: %s\n", argv[0], holdstring);*/

    return (0); /*** Successful execution on waitpid success ***/
}



/**
 * execute_cd - Execute the built-in cd command.
 *
 * @input: The input command.
 */
void execute_cd(char *input) {
    char *input_copy;
    char *path;
    char error_message[1024];
    int length;
    char *command;
    int number = 1;
    char prev_cwd[1024]; /* Buffer to store the previous directory */
    DIR *dir = NULL; /* Directory variable declaration */
    char **new_env;
    int ispathused = 0;

    /* Check if the standard input is a terminal (interactive mode) */
    int is_interactive = isInteractiveMode();

    /* Make a copy of the input string */
    input_copy = stringdup(input); /* Using your string duplication function */
    if (input_copy == NULL) {
        perror("strdup");
        return;
    }

    /* Tokenize the input to get the 'cd' commands and paths */
    command = stringtok(input_copy, "\n");

    while (command != NULL) {
        /* Check if the command is "cd" (with or without arguments) */
        if (startwith(command, "cd")) {
            if (strcondition(command, "cd", " ", 1) == 0) {
                path = get_environment("HOME");
		ispathused++;
            } else {
                /* Get the path from the command or use HOME if not provided */
                path = stringtok(command + 3, " \t");
                if (stringcmp(path, "~") == 0) {
                    /* Handle "cd ~" */
                    path = get_environment("HOME");
		    ispathused++;
                } else if (stringcmp(path, "-") == 0) {
                    /* Handle "cd -" */
                    path = get_environment("OLDPWD");
		    ispathused++;
                }
            }

            /* Get the current directory before changing */
            if (getcwd(prev_cwd, sizeof(prev_cwd)) == NULL) {
                perror("getcwd");
                free(input_copy); /* Free the allocated memory */
                return;
            }

            /* Change the current directory */
            if (chdir(path) != 0) {
                /* Handle the error */
                length = strinprintf(error_message, sizeof(error_message),
                    "./hsh: %d: cd: can't cd to %s\n", number, path);
                if (length < 0) {
                    perror("custom_snprintf");
                } else {
                    write(STDERR_FILENO, error_message, length);
		    free(input_copy);
		    return;
                }
            } else {
                /* Update the PWD and OLDPWD environment variables */
                if (is_interactive) {
                    new_env = set_environment("OLDPWD", prev_cwd, 1, 0);
                    if (new_env == NULL) {
                        perror("setenv");
                        free(input_copy); /* Free the allocated memory */
                        return;
                    }

                    new_env = set_environment("PWD", getcwd(prev_cwd, sizeof(prev_cwd)), 1, 0);
                    if (new_env == NULL) {
                        perror("setenv");
                        free(input_copy); /* Free the allocated memory */
                        return;
                    }

                }
            }
        }

        /* Move to the next command in the input string */
        command = stringtok(NULL, "\n");
        number++;
    }

    /* Optional: Close the directory stream if it's open */
    if (dir != NULL) {
        closedir(dir);
    }

    /* Free the allocated memory for the input copy */
    free(input_copy);
    if (ispathused) {
	free(path);
    }
    if (new_env != NULL && new_env != environ) {
	free_environment(new_env);
    }
	
}






/**
 * get_system - Execute a shell command using execve.
 *
 * @command: The command to execute.
 * Return: The exit status of the command.
 */
int get_system(char *command)
{
    pid_t pid;
    int status;
    int exit_status = 0; /** Initialize with an error value **/

    /** Create the environment for the child process **/
    char **envp = create_environment();

    /** Fork a new process **/
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        free_environment(envp);
        return (exit_status);
    }
    else if (pid == 0)
    {
        /** Child process: Parse and execute the command **/
        char *args[4];
        args[0] = "/bin/sh";
        args[1] = "-c";
        args[2] = (char *)command; /** Command to execute **/
        args[3] = NULL; /** Null-terminate the args array **/

        /** Execute the command using execve **/
       if (execve("/bin/sh", args, envp) == -1) {
          /** execve only returns if an error occurred **/
          free_environment(envp);
          handle_errno(command);

       }
    }
    else
    {
        /** Parent process: Wait for the child to complete **/
        free_environment(envp);
        waitpid(pid, &status, 0);

        if (WIFEXITED(status))
        {
            /** Child exited normally, return its exit status **/
            exit_status = WEXITSTATUS(status);
        }
    }

    return (exit_status);
}


/**
 * run_system - Execute a shell command using the system function.
 * @command: The command to execute.
 *
 * Return: The exit status of the command or -1 on error.
 */
int run_system(const char *command)
{
	int exit_status;

	/* Execute the command using the system function */
	exit_status = system(command);

     	if (exit_status == -1)
	{
	perror("system");
	return (-1); /* Error occurred while executing the command */
     }

  return (exit_status); /* Return the exit status of the command */
}
