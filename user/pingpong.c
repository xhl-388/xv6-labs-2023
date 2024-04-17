#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main()
{
    int fd[2];
    int pid;
    int buff;
    if (pipe(fd))
        fprintf(2, "Error create pipe\n");

    pid = fork();
    if (0 == pid) {
        // child
        if (sizeof(buff) != read(fd[0], &buff, sizeof(buff))) {
            fprintf(2, "Read failed\n");
            exit(1);
        }
        printf("%d: received ping\n", getpid());
        close(fd[0]);
        if (sizeof(buff) != write(fd[1], &buff, sizeof(buff))) {
            fprintf(2, "write failed\n");
        }
        close(fd[1]);
    } else if (pid > 0) {
        // parent
        buff = 3;
        if (sizeof(buff) != write(fd[1], &buff, sizeof(buff))) {
            fprintf(2, "write failed\n");
        }
        close(fd[1]);
        if (sizeof(buff) != read(fd[0], &buff, sizeof(buff))) {
            fprintf(2, "Read failed\n");
            exit(1);
        }
        printf("%d: received pong\n", getpid());
        close(fd[0]);
    } else {
        fprintf(2, "fork failed\n");
    }

    exit(0);
}