#include "kernel/types.h"
#include "user/user.h"

#define RD 0
#define WR 1


void recursive_filters(int reads)
{
    int p;
    if(read(reads , &p , sizeof(int)) == sizeof(int)){
        printf("prime %d\n" , p);
    }
    int temp_fd[2];     // 开一个新的管道
    pipe(temp_fd);
    int pid2 = fork();  // 每一个prime都开一个进程
    if(pid2 < 0){
        close(temp_fd[WR]);
        close(temp_fd[RD]);
        exit(1);
    }
    if(pid2 == 0){
        close(reads);
        close(temp_fd[WR]);
        recursive_filters(temp_fd[RD]);
        close(temp_fd[RD]);
        exit(0);
    }
    else{
        close(temp_fd[RD]);
        int n;
        while(read(reads , &n , sizeof(int)) == sizeof(int)){
            if(n % p != 0){
                write(temp_fd[WR] , &n , sizeof(int));
            }
        }
        close(temp_fd[WR]);
        close(reads);
        wait((int *) 0);
    }
    
}

int main()       //每一个进程有一个管道即可
{
    int pipe_fd[2];
    pipe(pipe_fd);

    int pid = fork();
    if(pid < 0){
        close(pipe_fd[RD]);
        close(pipe_fd[WR]);
        exit(1);
    }
    if(pid == 0){
        close(pipe_fd[WR]);
        recursive_filters(pipe_fd[RD]);
        exit(0);
    }
    else{
        close(pipe_fd[RD]);
        for(int i = 2 ; i <= 35 ; i++){    //直接写入int型的整数
            write(pipe_fd[WR] , &i , sizeof(int)); 
        }
        close(pipe_fd[WR]);
        wait((int *) 0);
        exit(0);
    }
}
