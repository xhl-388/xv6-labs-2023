#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
    char buf[512];
    char *args[MAXARG] = {0};
    int pid;
    int i = 0;

    while (i+1 < argc && argv[i+1] != 0) {
        args[i] = argv[i+1];
        i++;
    }
    
    memset(buf, 0, sizeof(buf));
    while (read(0, buf+strlen(buf), sizeof(buf)-strlen(buf)));

    int l = 0, n = strlen(buf);
    for (int r = 0; r < n; r++) {
        if(buf[r] == '\n') {
            args[i++] = &buf[l];
            buf[r] = '\0';
            l = r+1;
        }
    }
    
    pid = fork();
    if (pid == 0) {
        if (exec(argv[1], (char**)args)) {
            fprintf(2, "exec failed\n");
            exit(1);
        }
        exit(0);
    } else
        wait(0);
    exit(0);
}