/*
 * 初始化一个守护进程
 */
#include "apue.h"
#include <syslog.h>
#include <fcntl.h>
#ifdef BSD
#include <sys/time.h>
#endif
#include <sys/resource.h>

void daemonize(const char *cmd) {
    int i, fd0, fd1, fd2;
    pid_t pid;
    struct rlimit r1;
    struct sigaction sa;
    
    /* 1. clear file creation mask. */
    umask(0);

   /* 2. get maximum number of file descriptors. */
    if (getrlimit(RLIMIT_NOFILE, &r1) < 0)
        err_quit("%s: can't get file limit", cmd);

    /* 3.become a session leader to lose controlling TTY. */
    if ((pid = fork()) < 0) 
        err_quit("%s: can't fork", cmd);
    else if (pid != 0) /* parent */
        exit(0);

    setsid();

    /* 4. ensure future opens won't allocate controlling TTYs. */
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGHUP, &sa, NULL) < 0)
        err_quit("%s: can't ignore SIGHUP");
    if ((pid = fork()) < 0)
        err_quit("%s: can't fork", cmd);
    else if (pid != 0) /* parent */
        exit(0);

    /* 5. change the current working directory to the root.
     * so we won't prevent file systems from being unmounted.
     */
    if (chdir("/") < 0)
        err_quit("%s: can't change directory to /");

    /* 6. close all open file descriptors. */
    if (r1.rlim_max == RLIM_INFINITY)
        r1.rlim_max = 1024;
    for (i = 0; i < r1.rlim_max; i++)
        close(i);

    /* 7. attach file descriptors 0, 1, and 2 to /dev/null. */
    fd0 = open("/dev/null", O_RDWR);
    fd1 = dup(0);
    fd2 = dup(0);

    /* 8. initialize the log file. */
    openlog(cmd, LOG_CONS, LOG_DAEMON);
    if (fd0 != 0 || fd1 != 1 || fd2 != 2) {
        syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
        exit(1);
    }
}



