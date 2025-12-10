#include <unistd.h>      // System calls
#include <string.h>      
#include <stdlib.h>      // Standard library (exit)

#define WELCOME "Bienvenue dans le Shell ENSEA.\nPour quitter, tapez 'exit'.\n"
#define PROMPT "enseash % "

int main() {

    // Welcome msg
    write(1, WELCOME, strlen(WELCOME));
    // Show prompt
    write(1, PROMPT, strlen(PROMPT));

    while (1) {};
    
    exit (EXIT_SUCCESS);
}
