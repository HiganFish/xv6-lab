#include "kernel/types.h"
#include "user/user.h"

int main() {

    int f_to_c[2];
    pipe(f_to_c);
    int c_to_f[2];
    pipe(c_to_f);
    
    int pid = fork();
    if (pid == 0) {
        close(f_to_c[1]);
        close(c_to_f[0]);
        char buff[1];
        read(f_to_c[0], buff, sizeof buff);
        printf("%d: received ping\n", getpid());
        write(c_to_f[1], "@", 1);
        exit(0);
    } else {
        close(f_to_c[0]);
        close(c_to_f[1]);
        write(f_to_c[1], "@", 1);
        char buff[1];
        read(c_to_f[0], buff, sizeof buff);
        printf("%d: received pong\n", getpid());
        wait(0);
        exit(0);
    }
}
