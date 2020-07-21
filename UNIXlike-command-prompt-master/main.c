#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 80

int isOutputRedirected(char *args[], int tokens_no) {
    for (int i = 0; i < tokens_no; i++) {
        if ((strcmp(args[i], ">")) == 0) {
            return i;
        }
    }
    return 0;
}

int isInputRedirected(char *args[], int tokens_no) {
    for (int i = 0; i < tokens_no; i++) {
        if ((strcmp(args[i], "<")) == 0) {
            return i;
        }
    }
    return 0;
}

int isCommunicationViaPipe(char *args[], int tokens_no) {
    for (int i = 0; i < tokens_no; i++) {
        if ((strcmp(args[i], "|")) == 0) {
            return i;
        }
    }
    return 0;
}

int process_command_into_tokens(char *buff, char *args[]) {

    char *ptr = strtok(buff, " ");
    int idx = 0;
    while (ptr != NULL) {
        args[idx++] = ptr;
        ptr = strtok(NULL, " ");
    }

    args[idx] = NULL;
    return idx;
}

void execute_command(int tokens_no, char *args[]) {
    int dontWait = 0;
    if (strcmp(args[tokens_no - 1], "&") == 0) {
        dontWait = 1;
        args[tokens_no - 1] = NULL;
    }
    pid_t pid = fork();
    if (pid < 0) {
        fprintf(stderr, "fork error");
        return;
    }
    if (pid == 0) {
        int special_char = 0;
        if ((special_char = isInputRedirected(args, tokens_no))) {
            int desc = open(args[special_char + 1], O_RDONLY);
            if (desc < 0) {
                printf("error creating descriptor (input) \n");
                return;
            }
            dup2(desc, STDIN_FILENO);
            args[special_char] = NULL;
            execvp(args[0], args);
        }
        else if ((special_char = isOutputRedirected(args, tokens_no))) {
            int desc = open(args[special_char + 1], O_CREAT | O_RDWR, 0777);
            if (desc < 0) {
                printf("error creating descriptor (output) \n");
            }
            dup2(desc, STDOUT_FILENO);
            args[special_char] = NULL;
            execvp(args[0], args);
        }
        else if ((special_char = isCommunicationViaPipe(args, tokens_no))) {
            int pipefd[2];
            pipe(pipefd);
            pid_t piped_child = fork();
            if (piped_child > 0) {
                close(pipefd[0]);
                dup2(pipefd[1], STDOUT_FILENO);
                args[special_char] = NULL;
                execvp(args[0], args);
            }
            else if (piped_child == 0) {
                close(pipefd[1]);
                dup2(pipefd[0], STDIN_FILENO);
                execvp(args[special_char + 1], (args + special_char + 1));
            }
        }
        else {
            execvp(args[0], args);
        }
    }
    if (dontWait == 0) {
        int status;
        waitpid(pid, &status, 0);
    }
    else {
        printf("not waiting \n");
        fflush(stdout);
    }
}

void strip_command_from_newline(char buff[], int *tokens_no) {
    unsigned long length = strlen(buff);
    buff[length - 1] = '\0';
    *tokens_no -= 1;
}


int main(void) {
    size_t buff_size = MAX_LINE;
    int should_run = 1;

    // new command
    char *args[MAX_LINE/2 + 1];
    char buff[MAX_LINE];
    char *buff_ptr = buff;
    int tokens_no = 0;

    // archived command
    char *archived_args[MAX_LINE/2 + 1];
    char archived_buff[MAX_LINE];
    int archived_tokens_no = 0;
    archived_args[0] = "";
    args[0] = "";

    while (should_run) {
        printf("osh>");
        fflush(stdout);

        int chars_read = getline(&buff_ptr, &buff_size, stdin);
        if (chars_read <= 1) {
            continue;
        }
        strip_command_from_newline(buff, &tokens_no);

        if (strcmp(args[0], "!!") == 0) {
            if (strcmp(archived_args[0], "") == 0 || strcmp(archived_args[0], "!!") == 0 || strcmp(archived_args[0], "!!!") == 0) {
                printf("no archived command \n");
            }
            else {
                char *pos = archived_buff;
                int read = 0;
                for (int i = 0; i < archived_tokens_no; i++) {
                    read = printf("%s ", pos);
                    pos += read;
                }
                printf("\n");
            }
        }
        else if (strcmp(args[0], "!!!") == 0) {
            if (strcmp(archived_args[0], "") == 0 || strcmp(archived_args[0], "!!") == 0 || strcmp(archived_args[0], "!!!") == 0) {
                printf("no archived command to execute \n");
            }
            else {
                execute_command(archived_tokens_no, archived_args);
            }
        }
        else if (strcmp(buff, "exit") == 0) {
            should_run = 0;
        }
        else {
            strcpy(archived_buff, buff);
            process_command_into_tokens(archived_buff, archived_args);

            tokens_no = process_command_into_tokens(buff, args);
            archived_tokens_no = tokens_no;
            execute_command(tokens_no, args);
        }

    }
    return 0;
}