/////////////////////////////////////////////////////////////////////////////////////
//    like linked list,a pipe is two pointer, a process is a node,
//in_fd is a pointer which the original process which have out_fd,it point to all the process if the process have it,and all process can opread it to close it.
// must free no use pointer or you will have many pointers
// it's hard to read using functions, write a graph using above ideas
// read zuse is soloved :
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define MAX 36
#define FIRST_PRIME 2

int generate_natural();                 // -> out_fd
int prime_filter(int in_fd, int prime); // -> out_fd

int main(int argc, char *argv[])
{
    int prime;

    int in = generate_natural();
    while (read(in, &prime, sizeof(int)))
    {
        // printf("prime %d: in_fd: %d\n", prime, in);  // debug
        printf("prime %d\n", prime);
        in = prime_filter(in, prime);
    }

    exit(0);
}

// 生成自然数: 2, 3, 4, ..< MAX
int generate_natural()
{
    int out_pipe[2];

    pipe(out_pipe);

    if (!fork())
    {
        for (int i = FIRST_PRIME; i < MAX; i++)
        {
            write(out_pipe[1], &i, sizeof(int));
        }
        close(out_pipe[1]);

        exit(0);
    }

    close(out_pipe[1]);

    return out_pipe[0];
}

// 素数筛
int prime_filter(int in_fd, int prime)
{
    int num;
    int out_pipe[2];

    pipe(out_pipe);

    if (!fork())
    {
        while (read(in_fd, &num, sizeof(int)))
        {
            if (num % prime)
            {
                write(out_pipe[1], &num, sizeof(int));
            }
        }
        close(in_fd);
        close(out_pipe[1]);

        exit(0);
    }

    close(in_fd);
    close(out_pipe[1]);

    return out_pipe[0];
}

/////////////////////////////////////////////////////////////////////////////////////
//   my solution ,can pass the test if you know how many primes before shai sushu
//                    only one pipe,bad                                            //
// #include "kernel/types.h"
// #include "kernel/stat.h"
// #include "user/user.h"

// int main(int argc, char *argv[])
// {
//     int fd_pipe[2];
//     int root_pid = getpid();
//     pipe(fd_pipe);
//     for (int i = 2; i < 36; i++)
//     {
//         char c = (char)i;
//         char *str = "0";
//         str[0] = c;
//         write(fd_pipe[1], str, 1); // 写入字符和终止符
//     }

//     for (int i = 0; i < 11; i++) // must know how many primes before calculate,may bug here because read pipe of no char
//     {
//         if (root_pid == getpid())
//         {
//             int pid = fork();
//             if (pid == 0) // new process or next process
//             {
//                 // p = get a number from left neighbor
//                 // print p
//                 // loop:
//                 //     n = get a number from left neighbor
//                 //     if (p does not divide n)
//                 //         send n to right neighbor
//                 char buff[50];
//                 int x = 1;
//                 while ((x = read(fd_pipe[0], buff, sizeof(buff))) == 1)
//                 {
//                     // 0 char in fd_pipe
//                     int p = (int)buff[0];
//                     fprintf(2, "prime %d\n", p);
//                     exit(0);
//                 }
//                 close(fd_pipe[0]);

//                 int p = (int)buff[0];
//                 fprintf(2, "prime %d\n", p);

//                 for (int i = 0; i < x; i++)
//                 {
//                     int n = (int)buff[i];
//                     char *str = "0";
//                     str[0] = buff[i];
//                     if (!(n % p == 0))
//                     {
//                         write(fd_pipe[1], str, 1);
//                     }
//                 }
//                 close(fd_pipe[1]);
//                 exit(0);
//             }
//             else
//             {
//                 wait((int *)0);
//             }
//         }
//     }
//     exit(0);
// }
