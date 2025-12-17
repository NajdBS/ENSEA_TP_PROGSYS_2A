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

//unction Prototypes
int fortune(int *time_ms);
int show_date(int *time_ms);

int main() {
    char buf[Buff_size];
    int len;
    int status;
    char prompt[Buff_size] = PROMPT; // Initialize with default prompt
    int exectime;
    
    // Display welcome message
    write(1, WELCOME, strlen(WELCOME));
    
    while (1) {
        //Display the prompt 
        write(1, prompt, strlen(prompt));
        
        // Read user input
        len = read(0, buf, Buff_size);
        
        // Handle EOF (Ctrl+D)
        if (len == 0) { 
            write(1, BYE, strlen(BYE));
            break;
        }

        // Remove the trailing newline character '\n' from the input
        if (len > 0) buf[len - 1] = '\0';

        //'exit' command
        if (strcmp(buf, "exit") == 0) {
            write(1, BYE, strlen(BYE));
            break;
        }
        //'fortune' command
        else if (strcmp(buf, "fortune") == 0) {
            status = fortune(&exectime);
            
            // Update prompt based on child status
            if (WIFEXITED(status)) {
                // Child exited normally
                sprintf(prompt, "enseash [exit:%d | %d MS] %% ", WEXITSTATUS(status), exectime);
            } else if (WIFSIGNALED(status)) {
                // Child was killed by a signal
                sprintf(prompt, "enseash [sign:%d | %d MS] %% ", WTERMSIG(status), exectime);
            }
        }
        // 'empty space' to show date
        else if (strcmp(buf, " ") == 0 || strcmp(buf, "date") == 0) { 
            status = show_date(&exectime);

            // Update prompt based on child status
            if (WIFEXITED(status)) {
                sprintf(prompt, "enseash [exit:%d | %d MS] %% ", WEXITSTATUS(status), exectime);
            } else if (WIFSIGNALED(status)) {
                sprintf(prompt, "enseash [sign:%d | %d MS] %% ", WTERMSIG(status), exectime);
            }
        }
        //Unknown command
        else { 
            write(1, ERR, strlen(ERR));
            sprintf(prompt, "%s", PROMPT); 
        }
    }
    return EXIT_SUCCESS;
}

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