#include "kernel/types.h"
#include "user/user.h"


int main()
{
    int p[2]; 
    pipe(p);   // 创建一个管道
    if(fork() == 0){    // 创建子程序
        char buf;
        if(read(p[0], &buf, 1) == 1){  // 当管道中无data时,read会等待
            close(p[0]);
            int pid = getpid();
            printf("%d: received ping\n", pid);
            write(p[1], &buf, 1);
            close(p[1]);
        }    
        exit(0);
    }
    else{
        char buf = 'a';
        write(p[1], &buf, 1);
        wait((int *) 0);
        if(read(p[0], &buf, 1) == 1){
            int pid = getpid();
            printf("%d: received pong\n", pid);
            close(p[0]);
            close(p[1]);
        }
    }
    exit(0);
}