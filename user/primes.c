#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
    int i;
    int base = 0;
    int pid = 0;
    int lfd[2];
    int rfd[2];
    for (i = 2; i <= 35; i++) {
        if (0 == pid) {
            // no child yet
            if(pipe(rfd)) {
                fprintf(2, "pipe failed\n");
                exit(1);
            }
            pid = fork();
            if (pid > 0) {
                // parent
                close(rfd[0]);
                write(rfd[1], &i, sizeof(i));
            } else if (0 == pid) {
                // child
                memcpy(lfd, rfd, sizeof(rfd));
                memset(rfd, 0, sizeof(rfd));
                close(lfd[1]);
                read(lfd[0], &base, sizeof(base));
                printf("prime %d\n", base);
                break;
            } else {
                fprintf(2, "fork failed\n");
                exit(1);
            }
        } else {
            write(rfd[1], &i, sizeof(i));
        }
    }
    if (0 == base) {
        // origin parent
        close(rfd[1]);
        wait(0);
    } else {
        while (0 != read(lfd[0], &i, sizeof(i))) {
            if (0 != i % base) {
                if (0 == pid) {
                    // no child yet
                    if(pipe(rfd)) {
                        fprintf(2, "pipe failed\n");
                        exit(1);
                    }
                    pid = fork();
                    if (pid > 0) {
                        // parent
                        close(rfd[0]);
                        write(rfd[1], &i, sizeof(i));
                    } else if (0 == pid) {
                        // child
                        close(lfd[0]);
                        memcpy(lfd, rfd, sizeof(rfd));
                        memset(rfd, 0, sizeof(rfd));
                        close(lfd[1]);
                        read(lfd[0], &base, sizeof(base));
                        printf("prime %d\n", base);
                    } else {
                        fprintf(2, "fork failed\n");
                        exit(1);
                    }
                } else {
                    write(rfd[1], &i, sizeof(i));
                }
            }
        }
        close(lfd[0]);
        close(rfd[1]);
        wait(0);
    }
    exit(0);
}