#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){
    int pid = fork();
    char *argv[] = {"."};

    printf("Se regresa para aca el proceso\n");

    if (pid > 0){
        printf("hello1\nfather here %d\n", pid);
        wait(&pid);
    }else{
        printf("hello2\nchild here %d\n", pid);
        sleep(4);
        execve("/bin/ls", argv, NULL); //Este sirve para ejecutar otro programa
    }

    return 0;
    
}