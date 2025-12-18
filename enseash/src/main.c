#include "../inc/shell.h"


int main() {
    char buf[BUFF_SIZE];
    char *argv[MAX_ARGS];
    char prompt[BUFF_SIZE];
    ssize_t len;

    // Initialize default prompt
    sprintf(prompt, "%s %% ", PROMPT_BASE);
    display_message(WELCOME_MSG);

    while (1) {
        
        display_message(prompt);
        
        // Read input from user
        len = read(STDIN_FILENO, buf, BUFF_SIZE);

        // Handle End of File (Ctrl+D)
        if (len <= 0) {
            display_message(BYE_MSG);
            break;
        }

        // Replace the newline character with a null terminator
        if (buf[len - 1] == '\n') {
            buf[len - 1] = '\0';
        }

        // Check for 'exit' command
        if (strcmp(buf, EXIT_CMD) == 0) {
            display_message(BYE_MSG);
            break;
        }

        // process input
        if (strlen(buf) > 0) {
            parse_command(buf, argv);
            execute_command(argv, prompt);
            
        }
    }

    return EXIT_SUCCESS;
}