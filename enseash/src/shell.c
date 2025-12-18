#include "../inc/shell.h"

void display_message(const char *msg) {
    write(STDOUT_FILENO, msg, strlen(msg));
}

void parse_command(char *buf, char **argv) {
    int i = 0;
    // Split the string using space as a delimiter
    argv[i] = strtok(buf, " ");
    while (argv[i] != NULL && i < MAX_ARGS - 1) {
        i++;
        argv[i] = strtok(NULL, " ");
    }
    // Last element must be NULL for execvp
    argv[i] = NULL;
}

long get_elapsed_time_ms(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000 + (end.tv_nsec - start.tv_nsec) / 1000000;
}


void handle_redirection(char **argv)
{
    for (int i = 0; argv[i] != NULL; i++) {

        // Output redirection 
        if (strcmp(argv[i], ">") == 0) {
            int fd = open(argv[i + 1],
                          O_WRONLY | O_CREAT | O_TRUNC,
                          ACCESS_MODE);
            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDOUT_FILENO);
            close(fd);

            argv[i] = NULL;   
        }

        //Input redirection 
        else if (strcmp(argv[i], "<") == 0) {
            int fd = open(argv[i + 1], O_RDONLY);
            if (fd < 0) {
                perror("open");
                exit(EXIT_FAILURE);
            }

            dup2(fd, STDIN_FILENO);
            close(fd);

            argv[i] = NULL;
        }
    }
}


void execute_command(char **argv, char *prompt) {
    struct timespec start, end;
    int status;
    pid_t pid;

    // Start tracking execution time
    clock_gettime(CLOCK_REALTIME, &start);

    pid = fork();
    if (pid == -1) {
        perror("fork");
    } else if (pid == 0) {
        //Child Process

        handle_redirection(argv);
        execvp(argv[0], argv);

        // This part is only reached if execvp fails
        display_message(ERR_MSG);
        exit(EXIT_FAILURE);
    } else {
        // Parent Process
        wait(&status);
        clock_gettime(CLOCK_REALTIME, &end);

        long exectime = get_elapsed_time_ms(start, end);

        // Update the prompt based on exit status or signal received
        if (WIFEXITED(status)) {
            sprintf(prompt, "%s [exit:%d|%ldms] %% ", PROMPT_BASE, WEXITSTATUS(status), exectime);
        } else if (WIFSIGNALED(status)) {
            sprintf(prompt, "%s [sign:%d|%ldms] %% ", PROMPT_BASE, WTERMSIG(status), exectime);
        }
    }
}