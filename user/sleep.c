#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        fprintf(2, "sleep: need nums of seconds\n");
        exit(0);
    }
    int nums = atoi(argv[1]);
    sleep(nums);
    exit(0);
}
