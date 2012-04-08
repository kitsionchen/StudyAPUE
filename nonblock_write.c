/* 长的非阻塞write */
#include "apue.h"
#include <fcntl.h>
#include <errno.h>

char buf[500000];

int main(void) {
    int n_to_write;
    int n_write;
    char *ptr;

    /*从标准输入读500000字节 */
    n_to_write = read(STDIN_FILENO, buf, sizeof(buf));
    fprintf(stderr, "read %d bytes\n", n_to_write);
    
    /* set nonblocking */
    set_fl(STDOUT_FILENO, O_NONBLOCK);

    ptr = buf;
    while (n_to_write > 0) {
        errno = 0;
        n_write = write(STDOUT_FILENO, ptr, n_to_write);
        fprintf(stderr, "write %d, errno = %d\n", n_write, errno);

        if (n_write > 0) {
            ptr += n_write;
            n_to_write -= n_write;
        }
    }
    /* clear nonblocking */
    clr_fl(STDOUT_FILENO, O_NONBLOCK);

    exit(0);
}
