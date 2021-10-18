#include "kernel/types.h"
#include "user/user.h"


int primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
int is_prime(int num) {
   for (int i = 0; i < sizeof(primes) / sizeof(int); ++i) {
        if (primes[i] == num) {
            return 1;
        } else if (primes[i] > num) {
            return 0;
        }
   } 
   return 0;
}

void copy1_to_2(int fd_in, int fd_out) {
    char buffer[32];
    int read_byte = 0;
    while ((read_byte = read(fd_in, buffer, sizeof(buffer))) > 0) {
        int write_byte = 0;
        int write_byte_temp = 0;
        while (write_byte < read_byte) {
            write_byte_temp = write(fd_out, buffer + write_byte,
                                    read_byte - write_byte);
            if (write_byte_temp < 0) {
                printf("error write\n");
                exit(-1);
            } else {
                write_byte += write_byte_temp;
            }
        }
    }
}

void proc_child(int* fds) {
    close(fds[1]);
    int number = 0;
    int has_prime = 0;
    while (read(fds[0], (char*)&number, sizeof(number)) != 0) {
        if (is_prime(number)) {
            printf("prime %d\n", number);
            has_prime = 1;
            break;
        } 
    }
    if (has_prime == 0) {
        exit(0);
    }
    int fds_child[2];
    pipe(fds_child);
    int pid = fork();
    if (pid == 0) {
        proc_child(fds_child);    
    } else {
        close(fds_child[0]);

        copy1_to_2(fds[0], fds_child[1]);

        close(fds[0]);
        close(fds_child[1]);
        wait(0);
    }
}

int main() {
    
    int fds[2];
    pipe(fds);
    int pid = fork();
    if (pid == 0) {
        proc_child(fds);
    } else {
        close(fds[0]);
        for (int i = 2; i <= 35; ++i) {
            write(fds[1], (char*)&i, sizeof(i));
        }
        close(fds[1]);
        wait(0);
    }
    exit(0);
}
