#include "apue.h"
#include <stdlib.h>
#include <string.h>


int main(int argc, char **argv) {
    char *cmd;
    if ((cmd = strrchr(argv[0], '/')) == NULL)
        cmd = argv[0];
    else
        cmd++;
    daemonize(cmd);

    exit(0);
}
