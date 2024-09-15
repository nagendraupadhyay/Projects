//Nagendra Upadhyay. CS360 lab8 jsh.c. This code below does shell operation in its own way.  
//The jsh shell is be able to execute commands, redirect input/output, and handle operations such as <, >, >>, &, and |
//This program uses various libraries for I/O, processes, and file control
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//The function below prompts the user for input and returns the input as a string
char* get_input(char* prompt) {
    // Allocate memory for the input string
    char* line = malloc(1024 * sizeof(char));
    // Read a line of input from standard input
    fgets(line, 1024, stdin);
    return line;
}

//This function executes a command that contains pipes
int execute_piped_commands(char* command[], int in_fd, int out_fd) {
    //Create a new process
    pid_t pid = fork();
    //If fork failed, print an error message and return 1
    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        //If this is the child process, redirect input and output if necessary
        if (in_fd != STDIN_FILENO) {
            dup2(in_fd, STDIN_FILENO);
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            dup2(out_fd, STDOUT_FILENO);
            close(out_fd);
        }
        //Execute the command
        execvp(command[0], command);
        //If execvp failed, print an error message and exit
        perror(command[0]);
        exit(1);
    } else {
        //Close input and output file descriptors if necessary
        if (in_fd != STDIN_FILENO) {
            close(in_fd);
        }
        if (out_fd != STDOUT_FILENO) {
            close(out_fd);
        }
    }
    //Return the process ID of the child process
    return pid;
}

int main(int argc, char** argv) {
    //Initialize the prompt to "jsh: "
    char* prompt = "jsh: ";
    //Check if the user passed in an argument
    if (argc > 2) {
        //If more than 2 arguments are passed, print the usage and exit
        fprintf(stderr, "Usage: %s [PROMPT]\n", argv[0]);
        return 1;
    } else if (argc == 2) {
        //If only 1 argument is passed, check if it is "-"
        //If it is "-", set prompt to an empty string
        //Otherwise, set prompt to the passed in argument
        if(strcmp(argv[1], "-") == 0){
            prompt = "";
        }else{
            prompt = argv[1];
        }
    }

    while (1) {
        //Get input from the user with the current prompt
        char* line = get_input(prompt);
        //Check if the user entered "exit" or reached end of input
        //If yes, free the input buffer and break the loop
        if (feof(stdin) || strcmp(line, "exit\n") == 0) {
            free(line);
            break;
        }

        //Check if the user wants to run the command in the background
        int background = 0;
        if (line[strlen(line) - 2] == '&') {
            background = 1;
            line[strlen(line) - 2] = '\0';
        }

        //Parse the input into individual commands
        int num_commands = 0;
        char* commands[128];
        char* token = strtok(line, "|");
        while (token != NULL) {
            commands[num_commands++] = token;
            token = strtok(NULL, "|");
        }

        //Initialize file descriptor for input to standard input
        int in_fd = STDIN_FILENO;
        //Initialize file descriptor array for pipes
        int fds[2];
        //Loop through each command and execute them
        for (int i = 0; i < num_commands; ++i) {
            //If it is not the last command, create a pipe
            if (i < num_commands - 1) {
                pipe(fds);
            } else {
                //If it is the last command, set the output to standard output
                fds[1] = STDOUT_FILENO;
            }

            //Parse each command into individual arguments and files
            int num_args = 0;
            char* args[128];
            char* input_file = NULL;
            char* output_file = NULL;
            int output_mode = 0; // 0: overwrite, 1: append
            char* command_token = strtok(commands[i], " \t\n");
            while (command_token != NULL) {
                if (strcmp(command_token, "<") == 0) {
                    //Input file specified
                    command_token = strtok(NULL, " \t\n");
                    input_file = command_token;
                    command_token = strtok(NULL, " \t\n");
                } else if (strcmp(command_token, ">") == 0) {
                    //Output file specified, overwrite mode
                    command_token = strtok(NULL, " \t\n");
                    output_file = command_token;
                    output_mode = 0;
                    command_token = strtok(NULL, " \t\n");
                } else if (strcmp(command_token, ">>") == 0) {
                    //Output file specified, append mode
                    command_token = strtok(NULL, " \t\n");
                    output_file = command_token;
                    output_mode = 1;
                    command_token = strtok(NULL, " \t\n");
                } else {
                    //Otherwise, add argument to array
                    args[num_args++] = command_token;
                    command_token = strtok(NULL, " \t\n");
                }
            }
            //Add null terminator to the end of the arguments array
            args[num_args] = NULL;
            //Flush standard input, output, and error streams
            fflush(stdin);
            fflush(stdout);
            fflush(stderr);

            //Open input file if specified
            if (input_file != NULL) {
                in_fd = open(input_file, O_RDONLY);
                if (in_fd < 0) {
                    perror(input_file);
                    break;
                }
            }
            //Open output file if specified
            if (output_file != NULL) {
                //If output mode is overwrite, open file with write-only, create, truncate flags
                if (output_mode == 0) {
                    fds[1] = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                } else {
                    //If output mode is append, open file with write-only, create, append flags
                    fds[1] = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
                }
                //Print error and exit if file open fails
                if (fds[1] < 0) {
                    perror(output_file);
                    break;
                }
            }

            //Execute piped commands with the given arguments, input file descriptor, and output file descriptor
            int pid = execute_piped_commands(args, in_fd, fds[1]);
            //Close input file descriptor if not the first command in the pipeline
            if (i > 0) {
                close(in_fd);
            }
            
            //Update input file descriptor to be the read end of the output pipe for the next command and close
            in_fd = fds[0];
            if (i < num_commands - 1) {
                close(fds[1]);
            }
            
            //If the process shouldn't run in the background and it's the last command, it waits for the child process to complete
            if (!background && i == num_commands - 1) {
                int status;
                waitpid(pid, &status, 0);
            }
        }
        //This line ends the for loop that iterates through each command and frees memory used by line.
        free(line);
    }
    return 0;
}
