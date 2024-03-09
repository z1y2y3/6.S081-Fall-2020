#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// echo hello world\n hello\n hello | xargs echo hello bitch

int main(int argc, char *argv[])
{
    // parse xargs
    char *file = argv[1];
    char *arg[argc - 1];
    for (int i = 0; i < argc - 1; i++)
    {
        arg[i] = argv[i + 1];
    }
    // parse fd
    char buff[200];
    int index[20];
    int nums = 0;
    int size = 0;
    size = read(0, buff, sizeof(buff));
    for (int i = 0; i < size; i++)
    {
        if ((buff[i] == '\n')) // may bug because i+1 yuejie
        {
            index[nums] = i;
            nums++;
        }

        if ((i == size - 1) && (buff[i] != '\n'))
        {
            index[nums] = i + 1;
            nums++;
        }
        // printf("%c",buff[i]);
    }
    
    // printf("%d\n", size);
    // printf("%d\n", nums);

    for (int i = 0; i < nums; i++)
    {
        if (!fork())
        {
            char buffer[200];

            int start;
            if (i == 0)
            {
                start = 0;
            }
            else
            {
                if (buff[index[i - 1]] == '\n')
                {
                    start = index[i - 1] + 2;
                }
                else
                {
                    start = index[i - 1] + 1;
                }
            }
            int end = index[i];

            int x = start;
            int y = 0;
            for (; x < end; x++)
            {
                buffer[y] = buff[x];
                y++;
            }
            buffer[y] = '\0';
            char *str = buffer;

            char *a[argc + 1];
            for (int i = 0; i < argc - 1; i++)
            {
                a[i] = arg[i];
                // printf("%s", a[i]);
            }
            a[argc - 1] = str;
            // printf("%s", a[argc - 1]);
            // printf("\n");
            a[argc] = 0;

            exec(file, a);
        }
        else
        {
            wait(0);
            // printf("%d", i);
        }
    }
    // printf(" end%d", 1);
    exit(0);
}
