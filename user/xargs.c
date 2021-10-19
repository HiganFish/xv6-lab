#include "kernel/types.h"
#include "user/user.h"

void run_command(char** argv_copy) {
    int pid = fork();
    if (pid == 0) {
        exec(argv_copy[0], argv_copy);
        printf("exec error\n");
        exit(0);
    } else {
        wait(0);
    }
}
int main(int argc, char* argv[]) {
    if (argc <= 1) {
        fprintf(2, "Wrong number of parameters\n");
    }
    char* a = 0;
    char** argv_copy = (char**)malloc(sizeof(a) * (argc + 1));
    for (int i = 1; i < argc; ++i) {
        argv_copy[i - 1] = argv[i];
    }
    argv_copy[argc] = 0;
    
    sleep(20);

    char buffer[512];
    memset(buffer, '\0', sizeof buffer);
    int len = read(0, buffer, sizeof buffer);

    char* begin = buffer;
    while (begin < buffer + len) {
        char* end = strchr(begin, '\n');
        if (end && end - buffer <= len) {
            *end = '\0';
            argv_copy[argc - 1] = begin;
            run_command(argv_copy);
            begin = end + 1;
        } else {
            argv_copy[argc - 1] = begin;
            run_command(argv_copy);
            break;
        }
    }
    exit(0);
}
