#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(const char* path, const char* file_name) {

    int fd = open(path, 0);
    if (fd < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }
    struct stat st;
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    int path_len = strlen(path);
    int file_name_len = strlen(file_name);
    switch (st.type) {
    case T_FILE:
        if (path_len < file_name_len) {
            break;
        }
        if (strcmp(path + path_len - file_name_len, file_name) == 0) {
            printf("%s\n", path);
        }
        break;

    case T_DIR: ;
        char buffer[512];
        if (path_len + 1 + DIRSIZ + 1 > sizeof(buffer)) {
            fprintf(2, "find: path too long\n");
            break;
        }
        strcpy(buffer, path);
        char* p = buffer + strlen(buffer);
        *p++ = '/';
        struct dirent de;
        while (read(fd, &de, sizeof de) == sizeof(de)) {
            if (de.inum == 0) {
                continue;
            }
            if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
                continue;
            }
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            find(buffer, file_name);
        }
        break;
    }
    close(fd);
}

int main(int argc, char* argv[]) {
    
    if (argc != 3) {
        fprintf(2, "wrong number of parameters %d\n", argc);
        exit(-1);
    }
    const char* path = argv[1];
    const char* file_name = argv[2];

    find(path, file_name);

    exit(0);
}
