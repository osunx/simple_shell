# 0x16. C - Simple Shell

![Shell Image](https://s3.amazonaws.com/intranet-projects-files/holbertonschool-low_level_programming/235/shell.jpeg)

## Description

This project is focused on creating a simple UNIX command line interpreter, commonly known as a shell. The shell should be capable of handling various commands and executing them, similar to the `/bin/sh` shell. It should also support some built-in commands and features.

## Output

Unless specified otherwise, your program must have the exact same output as `/bin/sh` as well as the exact same error output. The only difference is when you print an error, the name of the program must be equivalent to your `argv[0]`.

### Example of error with `/bin/sh`:

```bash
$ echo "qwerty" | /bin/sh
/bin/sh: 1: qwerty: not found
$ echo "qwerty" | /bin/../bin/sh
/bin/../bin/sh: 1: qwerty: not found
Same error with your program (hsh):
bash
Copy code
$ echo "qwerty" | ./hsh
./hsh: 1: qwerty: not found
$ echo "qwerty" | ./././hsh
./././hsh: 1: qwerty: not found
List of Allowed Functions and System Calls
access (man 2 access)
chdir (man 2 chdir)
close (man 2 close)
closedir (man 3 closedir)
execve (man 2 execve)
exit (man 3 exit)
_exit (man 2 _exit)
fflush (man 3 fflush)
fork (man 2 fork)
free (man 3 free)
getcwd (man 3 getcwd)
getline (man 3 getline)
getpid (man 2 getpid)
isatty (man 3 isatty)
kill (man 2 kill)
malloc (man 3 malloc)
open (man 2 open)
opendir (man 3 opendir)
perror (man 3 perror)
read (man 2 read)
readdir (man 3 readdir)
signal (man 2 signal)
stat (__xstat) (man 2 stat)
lstat (__lxstat) (man 2 lstat)
fstat (__fxstat) (man 2 fstat)
strtok (man 3 strtok)
wait (man 2 wait)
waitpid (man 2 waitpid)
wait3 (man 2 wait3)
wait4 (man 2 wait4)
write (man 2 write)
Compilation
To compile your shell, use the following command:

gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
Testing
Your shell should work both in interactive and non-interactive modes.

Interactive mode:

$ ./hsh
($) /bin/ls
hsh main.c shell.c
($)
($) exit
$
Non-interactive mode:

$ echo "/bin/ls" | ./hsh
hsh main.c shell.c test_ls_2
## Tasks

### 0. Betty would be proud

Write a beautiful code that passes the Betty checks.

### 1. Simple shell 0.1

Write a UNIX command line interpreter.

Usage: simple_shell

Your Shell should:

- Display a prompt and wait for the user to type a command. A command line always ends with a new line.
- The prompt is displayed again each time a command has been executed.
- The command lines are simple, no semicolons, no pipes, no redirections or any other advanced features.
- The command lines are made only of one word. No arguments will be passed to programs.
- If an executable cannot be found, print an error message and display the prompt again.
- Handle errors.
- You have to handle the "end of file" condition (Ctrl+D).

### 2. Simple shell 0.2

Handle command lines with arguments.

### 3. Simple shell 0.3

Handle the PATH.
fork must not be called if the command doesn't exist.

### 4. Simple shell 0.4

Implement the exit built-in, that exits the shell.

Usage: exit
You don't have to handle any argument to the built-in exit.

### 5. Simple shell 1.0

Implement the env built-in, that prints the current environment.

### 6. Simple shell 0.1.1

Write your own getline function.

### 7. Simple shell 0.2.1

You are not allowed to use strtok.

### 8. Simple shell 0.4.1

Handle arguments for the built-in exit.

### 9. setenv, unsetenv

Implement the setenv and unsetenv builtin commands.

### 10. cd

Implement the builtin command cd.

### 11. ;

Handle the commands separator ;.

### 12. && and ||

Handle the shell logical operators && and ||.

### 13. alias

Implement the alias builtin command.

### 14. Variables

Handle variables replacement.
Handle the $? variable.
Handle the $$ variable.

### 15. Comments

Handle comments (#).

### 16. File as input

Your shell can take a file as a command line argument.

The file contains all the commands that your shell should run before exiting.

In this mode, the shell should not print a prompt and should not read from stdin.
## End of task

## Collaborators
## @osunx,
