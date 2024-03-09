#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char *path, char *filename)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }
    switch (st.type)
    {
    case T_FILE:
        printf("%s", path);
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("ls: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("ls: cannot stat %s\n", buf);
                continue;
            }
            switch (st.type)
            {
            case T_FILE:
                if (!strcmp(p, filename))
                {
                    printf("%s\n", buf);
                }
                break;
            case T_DIR:
                if (!strcmp(p, ".") || !strcmp(p, "..")) ;
                else{
                    find(buf,filename);
                }
                
                break;
            }
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    // find filename和find path filename
    if (argc == 2)
    {
        find(".", argv[1]);
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}
