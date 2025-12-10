#include <unistd.h>      // System calls
#include <string.h>      
#include <stdlib.h>      // Standard library (exit)
#include <sys/wait.h>    


#define WELCOME "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define PROMPT "enseash % "
#define BYE "Bye bye...\n"
#define ERR "Error command.\n"

void fortune();
void show_date();

int main() {

    char buf[128];
    int len;
    //int status;
    //pid_t pid;
    
    
    // Welcome msg
    write(1, WELCOME, strlen(WELCOME));
    
    while (1) {
        // Show prompt
        write(1, PROMPT, strlen(PROMPT));

        // Read input
        len = read(0, buf, 128);

        // remove \n (ENTER)
        buf[len - 1] = '\0';

        // Check fortune
        if (strcmp(buf, "fortune") == 0) {
            fortune();
            continue; 
        }
        // Check fortune
        else if (strcmp(buf, " ") == 0) {
            show_date();
            continue; 
        }
        // Check exit
        else if (strcmp(buf, "exit") == 0) {
            write(1, BYE, strlen(BYE));
            break;
        }
        
        // Check EOF (Ctrl+D)
        else if (len == 0) { 
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

void fortune(){
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
        }
}
void show_date() {
    pid_t pid;
    int status;

    pid = fork();

    if (pid == 0) {

        execl("/bin/date", "date", (char *)NULL);

        write(1, ERR, strlen(ERR));
        exit(EXIT_FAILURE);

    } else {
       
        wait(&status);
    }
}