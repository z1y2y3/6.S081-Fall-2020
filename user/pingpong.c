#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int to_p[2];
    int to_c[2];
    pipe(to_p);
    pipe(to_c);
    if (fork() == 0) // child
    {
        // read a byte from parent
        char *info[1];
        close(to_c[1]);
        read(to_c[0], info, 1);
        close(to_c[0]);
        fprintf(2, "%d: received ping\n", getpid());
        // send this byte back to parent
        close(to_p[0]);
        write(to_p[1], info, 1);
        close(to_p[1]);
        exit(0);
    }
    else // parent
    {
        // send a byte to child
        close(to_c[0]);
        write(to_c[1], "a", 1);
        close(to_c[1]);
        wait((int *)0);
        // read a byte from child
        char *buff[1];
        close(to_c[1]);
        read(to_c[0], buff, 1);
        close(to_c[0]);
        fprintf(2, "%d: received pong\n", getpid());
        exit(0);
    }
}