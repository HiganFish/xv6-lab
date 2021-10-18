#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("wrong number of parameters\n");
    }
    int time = atoi(argv[1]);
    int ret = sleep(time);
    exit(ret);
}
