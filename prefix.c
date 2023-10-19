#include "main.h"


/* 
 *Define prefixs values.
*/
char *prefixes[] = {
    "nohup",      /* Run in the background, ignoring hangup signal */
    "cd",         /* Change the shell working directory */
    "umask",      /* Set the default file permissions */
    "trap",       /* Execute a command when the shell receives a signal */
    "eval",       /* Evaluate several commands/arguments */
    "exec",       /* Replace the shell with another command */
    "source",     /* Execute commands from a file */
    "return",     /* Exit a function */
    "shift",      /* Shift positional parameters */
    "exit",       /* Terminate the shell */
    "export",     /* Set an environment variable */
    "unset",      /* Unset an environment variable */
    "test",       /* Check file types and compare values */
    "alias",      /* Create an alias for a command */
    "bg",         /* Resume suspended jobs in the background */
    "fg",         /* Resume suspended jobs in the foreground */
    "jobs",       /* List active jobs */
    "wait",       /* Wait for a process to complete */
    "kill",       /* Send a signal to a process */
    "echo",       /* Display a message */
    "printf",     /* Format and print data */
    "read",       /* Read a line from standard input */
    "readonly",   /* Mark variables/functions as readonly */
    "ulimit",     /* Set or get user limits */
    "type",       /* Display information about command type */
    "times",      /* Display shell process times */
    "history",    /* Command History */
    "shopt",      /* Shell Options */
    "set",        /* Shell Settings */
    "dirs",       /* Display the list of currently remembered directories */
    "pushd",      /* Add directories to the directory stack */
    "popd",       /* Remove directories from the directory stack */
    "umask",      /* Set the default file permissions */
    "bg",         /* Put a job in the background */
    "fg",         /* Bring a job to the foreground */
    "case",       /* Conditionally perform a command */
    "coproc",     /* Co-process */
    "declare",    /* Declare variables and give them attributes */
    "elif",       /* Else if condition */
    "esac",       /* End case condition */
    "for",        /* For loop */
    "function",   /* Define a function */
    "if",         /* If condition */
    "select",     /* Generate a list of items */
    "then",       /* Then condition */
    "time",       /* Time a pipeline */
    "until",      /* Until loop */
    "while",      /* While loop */
    "do",         /* Do loop */
    "done",       /* End loop */
    ":",          /* No-op, or null command */
    "continue",   /* Skip the remaining commands in a loop and start the next iteration */
    NULL          /*  NULL pointer to indicate the end of the array  */
};


/**
 * startsWithPrefix - Check if a command starts with a prefix.
 * @prefixes: The execution of program format to check.
 * @input: The command to find.
 * Return: 0 if the command starts with a prefix, -1 otherwise.
 */

char *startsWithPrefix(char *input)
{
  size_t i = 0;
  size_t prefix_length;

  while (prefixes[i] != NULL) {	  
	prefix_length = stringlen(prefixes[i]);

	if (strncmp(input, prefixes[i], prefix_length) == 0) {
	   return (prefixes[i]);
        }

   i++;
  }

 return (NULL); /* No matching prefix found */
}


/* concatTokens - Concatenates all tokens into a single string.
 *
 * @tokens: The array of tokens to concatenate.
 * @separator: The character to use for concatenation. Pass '\0' for no separator (default).
 *
 * Return: (char *) A dynamically allocated string containing all tokens separated by the specified separator.
 */
char *concatTokens(char **tokens, char *separator)
{
    size_t i;
    char *concatenated;
    size_t total_length = 0;
    if (tokens == NULL || tokens[0] == NULL)
    {
        return (NULL); /* No tokens to concatenate */
    }

    for (i = 0; tokens[i] != NULL; i++)
    {
        total_length += strlen(tokens[i]);
    }

    /* Allocate memory for the concatenated string */
    if (separator != NULL)
    {
        concatenated = (char *)malloc(total_length + i + 1); /* +i for separators +1 for null terminator */
        if (concatenated == NULL)
        {
            perror("malloc");
            return (NULL);
        }

        concatenated[0] = '\0'; /* Initialize as an empty string */

        for (i = 0; tokens[i] != NULL; i++)
        {
            if (i > 0)
            {
                strcat(concatenated, separator);
            }
            strcat(concatenated, tokens[i]);
        }
    }
    else
    {
        concatenated = (char *)malloc(total_length + 1); /* +1 for null terminator */
        if (concatenated == NULL)
        {
            perror("malloc");
            return (NULL);
        }

        concatenated[0] = '\0'; /* Initialize as an empty string */

        for (i = 0; tokens[i] != NULL; i++)
        {
            strcat(concatenated, tokens[i]);
        }
    }

    return (concatenated);
}




/**
 * handle_errno - Handle errno and exit the program with an appropriate error message.
 *  
 * This function checks the global variable `errno` and exits the program with an appropriate error
 * message printed to stderr. It handles various error cases and provides descriptive error
 * messages for each errno value.
 * 
 * @program_name - Command form calling function.
 * 
 */

void handle_errno(char *program_name)
{
/* Get the error message associated with errno */
const char *error_message = strerror(errno);
size_t i;
    switch (errno)
    {
        case E2BIG:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Argument list too long\n", 26);
            exit(EXIT_FAILURE);

        case EACCES:
            write(STDERR_FILENO, "./hsh: ", 8);
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Permission denied\n", 20);
            exit(EXIT_FAILURE);

        case EADDRINUSE:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Address already in use\n", 24);
            exit(EXIT_FAILURE);

        case EADDRNOTAVAIL:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Address not available\n", 22);
            exit(EXIT_FAILURE);

        case EAFNOSUPPORT:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Address family not supported\n", 31);
            exit(EXIT_FAILURE);

       case EAGAIN:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Resource temporarily unavailable\n", 33);
            exit(EXIT_FAILURE);

        case EALREADY:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Connection already in progress\n", 32);
            exit(EXIT_FAILURE);

        case EBADF:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Bad file descriptor\n", 20);
            exit(EXIT_FAILURE);

        case EBADFD:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": File descriptor in bad state\n", 30);
            exit(EXIT_FAILURE);

        case EBADMSG:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Bad message\n", 12);
            exit(EXIT_FAILURE);

        case EBADR:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Invalid request descriptor\n", 27);
            exit(EXIT_FAILURE);

        case EBADRQC:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Invalid request code\n", 20);
            exit(EXIT_FAILURE);

        case EBADSLT:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Invalid slot\n", 13);
            exit(EXIT_FAILURE);

        case EBUSY:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Device or resource busy\n", 24);
            exit(EXIT_FAILURE);

        case ECANCELED:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Operation canceled\n", 19);
            exit(EXIT_FAILURE);

        case ECHILD:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No child processes\n", 19);
            exit(EXIT_FAILURE);

        case ECHRNG:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Channel number out of range\n", 29);
            exit(EXIT_FAILURE);

        case ECOMM:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Communication error on send\n", 30);
            exit(EXIT_FAILURE);

        case ECONNABORTED:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Connection aborted\n", 19);
            exit(EXIT_FAILURE);

        case ECONNREFUSED:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Connection refused\n", 19);
            exit(EXIT_FAILURE);

        case ECONNRESET:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Connection reset\n", 17);
            exit(EXIT_FAILURE);

        case EDEADLOCK:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Resource deadlock avoided.\n", 28);
            exit(EXIT_FAILURE);

        case EDESTADDRREQ:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Destination address required\n", 29);
            exit(EXIT_FAILURE);

        case EDOM:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Mathematics argument out of domain of function\n", 49);
            exit(EXIT_FAILURE);

        case EDQUOT:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Disk quota exceeded\n", 20);
            exit(EXIT_FAILURE);

        case EEXIST:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": File exists\n", 12);
            exit(EXIT_FAILURE);

        case EFAULT:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Bad address\n", 12);
            exit(EXIT_FAILURE);

        case EFBIG:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": File too large\n", 15);
            exit(EXIT_FAILURE);

        case EHOSTDOWN:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Host is down\n", 13);
            exit(EXIT_FAILURE);

        case EHOSTUNREACH:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Host is unreachable\n", 20);
            exit(EXIT_FAILURE);

        case EHWPOISON:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Memory page has hardware error\n", 31);
            exit(EXIT_FAILURE);

        case EIDRM:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Identifier removed\n", 19);
            exit(EXIT_FAILURE);

        case EILSEQ:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Invalid or incomplete multibyte or wide character\n", 53);
            exit(EXIT_FAILURE);

        case EINPROGRESS:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Operation in progress\n", 21);
            exit(EXIT_FAILURE);

        case EINTR:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Interrupted function call\n", 26);
            exit(EXIT_FAILURE);

        case EINVAL:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Invalid argument\n", 17);
            exit(EXIT_FAILURE);

        case EIO:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Input/output error\n", 19);
            exit(EXIT_FAILURE);

        case EISCONN:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Socket is connected\n", 20);
            exit(EXIT_FAILURE);

        case EISDIR:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Is a directory\n", 15);
            exit(EXIT_FAILURE);

        case EISNAM:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Is a named type file\n", 21);
            exit(EXIT_FAILURE);

        case EKEYEXPIRED:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Key has expired\n", 16);
            exit(EXIT_FAILURE);

        case EKEYREJECTED:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Key was rejected by service\n", 29);
            exit(EXIT_FAILURE);

        case EKEYREVOKED:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Key has been revoked\n", 21);
            exit(EXIT_FAILURE);

        case EL2HLT:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Level 2 halted\n", 15);
            exit(EXIT_FAILURE);

        case EL2NSYNC:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Level 2 not synchronized\n", 24);
            exit(EXIT_FAILURE);

        case EL3HLT:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Level 3 halted\n", 15);
            exit(EXIT_FAILURE);

        case EL3RST:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Level 3 reset\n", 14);
            exit(EXIT_FAILURE);

        case ELIBACC:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Cannot access a needed shared library\n", 39);
            exit(EXIT_FAILURE);

        case ELIBBAD:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Accessing a corrupted shared library\n", 37);
            exit(EXIT_FAILURE);

        case ELIBMAX:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Attempting to link in too many shared libraries\n", 51);
            exit(EXIT_FAILURE);

        case ELIBSCN:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": .lib section in a.out corrupted\n", 34);
            exit(EXIT_FAILURE);

        case ELIBEXEC:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Cannot exec a shared library directly\n", 40);
            exit(EXIT_FAILURE);

        case ELOOP:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Too many levels of symbolic links\n", 34);
            exit(EXIT_FAILURE);

        case EMEDIUMTYPE:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Wrong medium type\n", 18);
            exit(EXIT_FAILURE);

        case EMFILE:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Too many open files\n", 21);
            exit(EXIT_FAILURE);

        case EMLINK:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Too many links\n", 15);
            exit(EXIT_FAILURE);

        case EMSGSIZE:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Message too long\n", 17);
            exit(EXIT_FAILURE);

        case EMULTIHOP:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Multihop attempted\n", 19);
            exit(EXIT_FAILURE);

        case ENAMETOOLONG:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Filename too long\n", 18);
            exit(EXIT_FAILURE);

        case ENETDOWN:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Network is down\n", 17);
            exit(EXIT_FAILURE);

        case ENETRESET:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Connection aborted by network\n", 31);
            exit(EXIT_FAILURE);

        case ENETUNREACH:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Network unreachable\n", 20);
            exit(EXIT_FAILURE);

        case ENFILE:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Too many open files in system\n", 30);
            exit(EXIT_FAILURE);

        case ENOANO:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No anode\n", 9);
            exit(EXIT_FAILURE);

        case ENOBUFS:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No buffer space available\n", 25);
            exit(EXIT_FAILURE);

        case ENODATA:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No message is available on the STREAM head read queue\n", 56);
            exit(EXIT_FAILURE);

        case ENODEV:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No such device\n", 15);
            exit(EXIT_FAILURE);

        case ENOENT:
	   /* Check if the error message contains "cannot access" and "No such file or directory" */
	   if (strstr(error_message, "cannot access") != NULL && strstr(error_message, "No such file or directory") != NULL) {
	     /* Handle the case where part of the command can't be accessed */
	      write(STDERR_FILENO, program_name, strlen(program_name));
	      write(STDERR_FILENO, ": ", 2); 
	      write(STDERR_FILENO, error_message, strlen(error_message));
	      write(STDERR_FILENO, "\n", 1); 
	     }  else {
              /* Handle other cases with standard messages */
              write(STDERR_FILENO, "./hsh:", 6);
              write(STDERR_FILENO, " No such file or directory\n", 27);
              }
           return;

        case ENOEXEC:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Exec format error\n", 18);
            exit(EXIT_FAILURE);

        case ENOKEY:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Required key not available\n", 28);
            exit(EXIT_FAILURE);

        case ENOLCK:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No locks available\n", 20);
            exit(EXIT_FAILURE);

        case ENOLINK:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Link has been severed\n", 21);
            exit(EXIT_FAILURE);

        case ENOMEDIUM:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No medium found\n", 16);
            exit(EXIT_FAILURE);

        case ENOMEM:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Not enough space/cannot allocate memory\n", 40);
            exit(EXIT_FAILURE);

        case ENOMSG:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No message of the desired type\n", 31);
            exit(EXIT_FAILURE);

        case ENONET:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Machine is not on the network\n", 30);
            exit(EXIT_FAILURE);

        case ENOPKG:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Package not installed\n", 22);
            exit(EXIT_FAILURE);

        case ENOPROTOOPT:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Protocol not available\n", 23);
            exit(EXIT_FAILURE);

        case ENOSPC:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No space left on device\n", 23);
            exit(EXIT_FAILURE);

        case ENOSR:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No STREAM resources\n", 20);
            exit(EXIT_FAILURE);

        case ENOSTR:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Not a STREAM\n", 13);
            exit(EXIT_FAILURE);

        case ENOSYS:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Function not implemented\n", 25);
            exit(EXIT_FAILURE);

        case ENOTBLK:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Block device required\n", 21);
            exit(EXIT_FAILURE);

        case ENOTCONN:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": The socket is not connected\n", 28);
            exit(EXIT_FAILURE);

        case ENOTDIR:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Not a directory\n", 16);
            exit(EXIT_FAILURE);

        case ENOTEMPTY:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Directory not empty\n", 20);
            exit(EXIT_FAILURE);

        case ENOTRECOVERABLE:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": State not recoverable\n", 22);
            exit(EXIT_FAILURE);

        case ENOTSOCK:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Not a socket\n", 14);
            exit(EXIT_FAILURE);

        case ENOTTY:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Inappropriate I/O control operation\n", 36);
            exit(EXIT_FAILURE);

        case ENOTUNIQ:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Name not unique on network\n", 28);
            exit(EXIT_FAILURE);

        case ENXIO:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No such device or address\n", 26);
            exit(EXIT_FAILURE);

        case EOPNOTSUPP:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Operation not supported\n", 25);
            exit(EXIT_FAILURE);

        case EOVERFLOW:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Value too large to be stored in data type\n", 44);
            exit(EXIT_FAILURE);

        case EOWNERDEAD:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Owner died\n", 11);
            exit(EXIT_FAILURE);

        case EPERM:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Operation not permitted\n", 24);
            exit(EXIT_FAILURE);

        case EPFNOSUPPORT:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Protocol family not supported\n", 32);
            exit(EXIT_FAILURE);

        case EPIPE:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Broken pipe\n", 13);
            exit(EXIT_FAILURE);

        case EPROTO:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Protocol error\n", 16);
            exit(EXIT_FAILURE);

        case EPROTONOSUPPORT:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Protocol not supported\n", 24);
            exit(EXIT_FAILURE);

        case EPROTOTYPE:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Protocol wrong type for socket\n", 34);
            exit(EXIT_FAILURE);

        case ERANGE:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Result too large\n", 19);
            exit(EXIT_FAILURE);

        case EROFS:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Read-only file system\n", 23);
            exit(EXIT_FAILURE);

        case ESPIPE:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Invalid seek\n", 14);
            exit(EXIT_FAILURE);

        case ESRCH:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": No such process\n", 17);
            exit(EXIT_FAILURE);

        case ESTALE:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Stale NFS file handle\n", 24);
            exit(EXIT_FAILURE);

        case ETIME:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Timer expired\n", 15);
            exit(EXIT_FAILURE);

        case ETIMEDOUT:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Connection timed out\n", 22);
            exit(EXIT_FAILURE);

        case ETXTBSY:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Text file busy\n", 16);
            exit(EXIT_FAILURE);

        case EUCLEAN:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Structure needs cleaning\n", 26);
            exit(EXIT_FAILURE);

        case EUNATCH:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Protocol driver not attached\n", 32);
            exit(EXIT_FAILURE);

        case EUSERS:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Too many users\n", 17);
            exit(EXIT_FAILURE);

        case EXDEV:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Improper link\n", 16);
            exit(EXIT_FAILURE);

        case EXFULL:
            write(STDERR_FILENO, program_name, strlen(program_name));
            write(STDERR_FILENO, ": Exchange full\n", 15);
            exit(EXIT_FAILURE);

        default:

            if (stringcmp(program_name, "EOF") == 0) {
                write(STDERR_FILENO, "\n\n[Disconnected...]\n\n", 21);
		/* Free the definedalias memory */
    		if (definedalias != NULL) {
    		for (i = 0; definedalias[i] != NULL; i++) {
        	    free(definedalias[i]);
    		}
    		free(definedalias);
    		}
                exit(1);
            } else {
               write(STDERR_FILENO, program_name, strlen(program_name));
               write(STDERR_FILENO, ": Unknown error: ", 15);
	       if (errno) {
	           write(STDERR_FILENO, strerror(errno), strlen(strerror(errno)));
		}
               write(STDERR_FILENO, "\n", 1);
               exit(EXIT_FAILURE);
	    }
     
      }

}
