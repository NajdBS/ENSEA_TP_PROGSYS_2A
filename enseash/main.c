#include <unistd.h>      
#include <string.h>      
#include <stdlib.h>      
#include <sys/wait.h>    
#include <stdio.h> // sprintf

#define WELCOME "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define PROMPT "enseash % "
#define BYE "Bye bye...\n"
#define ERR "Error command.\n"
#define Buff_size 128

int fortune();
int show_date();

int main() {

    char buf[Buff_size];
    int len;
    int status;
    //pid_t pid;
    char prompt[Buff_size] = "enseash % ";
    
    
    // Welcome msg
    write(1, WELCOME, strlen(WELCOME));
    
    while (1) {

        // Show prompt
        //write(1, PROMPT, strlen(PROMPT));
        write(1, prompt, strlen(prompt));
        // Read input
        len = read(0, buf, Buff_size);
        
        // Check EOF (Ctrl+D)
        if (len == 0) { 
            write(1, BYE, strlen(BYE));
            break;
        }
        // remove \n (ENTER)
        buf[len - 1] = '\0';

        // Check fortune
        if (strcmp(buf, "fortune") == 0) {
            status = fortune();
            if (WIFEXITED(status)) {
                sprintf(prompt, "enseash [exit:%d] %% ", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                sprintf(prompt, "enseash [sign:%d] %% ", WTERMSIG(status));
            }
            continue; 
        }
        // Check date  
        else if (strcmp(buf, " ") == 0) {
            status = show_date();
            if (WIFEXITED(status)) {
                sprintf(prompt, "enseash [exit:%d] %% ", WEXITSTATUS(status));
            } else if (WIFSIGNALED(status)) {
                sprintf(prompt, "enseash [sign:%d] %% ", WTERMSIG(status));
            }
            continue; 
        }
        // Check exit
        else if (strcmp(buf, "exit") == 0) {
            write(1, BYE, strlen(BYE));
            break;
        }
        else { 
            write(1, ERR, strlen(ERR));
            continue;
        }
    }
    return EXIT_SUCCESS;
}

int fortune(){
    pid_t pid;
    int status;
    pid = fork();

        if (pid == 0) {
            // Child process
            execl("/bin/sh", "sh", "-c", "fortune", (char *)NULL);
            // ERROR
            write(1, ERR, strlen(ERR));
            exit(EXIT_FAILURE);
        } else {
            // Parent process
            wait(&status);
            return status;
        }
}
int show_date() {
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0) {

        execl("/bin/date", "date", (char *)NULL);

        write(1, ERR, strlen(ERR));
        exit(EXIT_FAILURE);

    } else {
       
        wait(&status);
        return status;
    }
}