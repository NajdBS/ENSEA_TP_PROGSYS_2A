#ifndef SHELL_H
#define SHELL_H

//Includes
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <fcntl.h>

// Macros
#define WELCOME_MSG "Welcome to ENSEA Shell.\nType 'exit' to quit.\n"
#define PROMPT_BASE "enseash"
#define EXIT_CMD "exit"
#define BYE_MSG "Bye bye...\n"
#define ERR_MSG "Error: command not found.\n"
#define BUFF_SIZE 128
#define MAX_ARGS 10
#define ACCESS_MODE 0644

// Function Prototypes

void display_message(const char *msg);

// Tokenizes the input buffer into arguments for execvp.
void parse_command(char *buf, char **argv);

// Calculates the difference between two timespec structures in milliseconds.
long get_elapsed_time_ms(struct timespec start, struct timespec end);

// Forks a process and executes the command. Updates the prompt with results.
void execute_command(char **argv, char *prompt);

//comment
void handle_redirection(char **argv);

#endif