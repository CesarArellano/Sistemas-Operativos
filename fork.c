#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    int pid = fork();
    if (pid == 0) {
        printf("hello\nfather here\n");
        wait();
    } else {
        printf("hello\nchild here\n");
        sleep(4);
    }
    return 0;
}