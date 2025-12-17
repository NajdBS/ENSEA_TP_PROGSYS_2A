#include <unistd.h>      
#include <string.h>      
#include <stdlib.h>      
#include <sys/wait.h>    
#include <stdio.h> 
#include <time.h> 
#include <sys/types.h>

#define WELCOME "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define PROMPT "enseash % "
#define BYE "Bye bye...\n"
#define ERR "Error command.\n"
#define Buff_size 128
#define MAX_ARGS 10  // Maximum number of arguments

//function Prototypes not used
int fortune(int *time_ms);
int show_date(int *time_ms);

int main() {
    char buf[Buff_size];
    int len;
    int status;
    char prompt[Buff_size] = PROMPT; 
    
    struct timespec start, end; 
    pid_t pid;
    long exectime; 
    
    // Array to store the command and its arguments
    char *argv[MAX_ARGS]; 
    int i;

    // Display welcome message
    write(STDOUT_FILENO, WELCOME, strlen(WELCOME));
    
    while (1) {
        // Display the prompt 
        write(STDOUT_FILENO, prompt, strlen(prompt));
        
        // Read user input
        len = read(STDIN_FILENO, buf, Buff_size);

        // Handle EOF (Ctrl+D)
        if (len == 0) { 
            write(STDOUT_FILENO, BYE, strlen(BYE));
            break;
        }

        // Remove the trailing newline character '\n'
        if (len > 0 && buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }

        // 'exit' command
        if (strcmp(buf, "exit") == 0) {
            write(STDOUT_FILENO, BYE, strlen(BYE));
            break;
        }
        
        // Parsing (Splitting string into tokens) 
        // strtok replaces spaces with \0 and returns pointers to tokens
        i = 0;
        argv[i] = strtok(buf, " "); // Get first token (command name)
        
        while (argv[i] != NULL && i < MAX_ARGS - 1) {
            i++;
            argv[i] = strtok(NULL, " "); // Get next tokens
        }
        // argv array MUST end with NULL for execvp
        argv[i] = NULL;


        // Start timer
        clock_gettime(CLOCK_REALTIME, &start);

        pid = fork();

        if (pid == -1) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            // Child Process 
            // Execute command using PATH
            //execlp(buf, buf, (char *)NULL);
            // Child: execvp takes the command name and the array of arguments
            execvp(argv[0], argv);
            
            // If we are here, execlp failed (command not found)
            write(STDOUT_FILENO, ERR, strlen(ERR));
            exit(EXIT_FAILURE);
            
        } else {
            // --- Parent Process ---
            wait(&status);
            
            // Stop timer
            clock_gettime(CLOCK_REALTIME, &end);
            
            // Calc duration in ms
            exectime = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;

            // Update prompt with exit code or signal
            if (WIFEXITED(status)) {
                sprintf(prompt, "enseash [exit:%d|%ldms] %% ", WEXITSTATUS(status), exectime);
            } else if (WIFSIGNALED(status)) {
                sprintf(prompt, "enseash [sign:%d|%ldms] %% ", WTERMSIG(status), exectime);
            }
        }
    }
    return EXIT_SUCCESS;
}

// we thought we should implemented each command in a separate function, but we merged the logic

/**
 * Function: fortune
 * -----------------
 * Description: Executes the 'fortune' command in a child process and measures
 * the execution time using clock_gettime.
 *
 * Input: 
 * int *time_ms : A pointer to an integer where the execution time 
 * (in milliseconds) will be stored.
 *
 * Return: 
 * int : The exit status (or signal status) of the child process 
 * returned by wait().
 */
int fortune(int *time_ms){
    pid_t pid;
    int status;
    struct timespec start, end;

    //Start the timer (Parent process)
    clock_gettime(CLOCK_REALTIME, &start); 
    
    pid = fork();

    if (pid == 0) {
        //Child Process
        // Execute the command (replaces the current process image)
        execl("/bin/sh", "sh", "-c", "fortune", (char *)NULL);
        
        // If execution reaches here, execl failed
        write(1, ERR, strlen(ERR));
        exit(EXIT_FAILURE);
    } else {
        //Parent Process
        wait(&status); // Wait for the child to finish
        
        //Stop the timer (Parent process)
        clock_gettime(CLOCK_REALTIME, &end); 
    
        //Calculate elapsed time in milliseconds
        *time_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
        
        return status;
    }
}

/**
 * Function: show_date
 * -------------------
 * Description: Executes the 'date' command in a child process and measures
 * the execution time.
 *
 * Input: 
 * int *time_ms : A pointer to an integer where the execution time 
 * (in milliseconds) will be stored.
 *
 * Return: 
 * int : The exit status (or signal status) of the child process 
 * returned by wait().
 */
int show_date(int *time_ms) {
    pid_t pid;
    int status;
    struct timespec start, end;

    //Start the timer
    clock_gettime(CLOCK_REALTIME, &start); 
    
    pid = fork();

    if (pid == 0) {
        //Child Process
        execl("/bin/date", "date", (char *)NULL);

        // If execution reaches here, execl failed
        write(1, ERR, strlen(ERR));
        exit(EXIT_FAILURE);
    } else {
        //Parent Process
        wait(&status);
        
        //Stop the timer
        clock_gettime(CLOCK_REALTIME, &end); 
        
        // Calculate elapsed time in milliseconds
        *time_ms = (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
 
        return status;
    }
}