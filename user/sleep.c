#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{
    int t = 0; 

    if (2 != argc) {
        fprintf(2, "Usage: sleep time ...\n");
        exit(1);
    }

    t = atoi(argv[1]);
    if (sleep(t)) {
        fprintf(2, "Error: proc being killed\n");
        exit(1);
    }
    exit(0);
}