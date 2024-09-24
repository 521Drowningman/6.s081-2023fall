#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

void xargs(char *argv[])
{
    int pid = fork();
    if(pid < 0){
        fprintf(2, "fork failed");
        exit(1);
    }
    if(pid == 0){
        // 第二个参数用 argv+1 的原因是:因为命令行参数的第一个元素应该命令本身.
        exec(argv[0], argv);
        fprintf(2, "exec failed");
        exit(0);
    }
    else {
        wait((int *)0);
    }

}

int main(int argc, char *argv[])
{
    if(argc < 2){
        fprintf(1, "Usage:xargs command\n");
        exit(1);
    }
    // 处理好xargs可能,能从标准输出中读取的参数
    char *line[MAXARG] = {0};    // 将指针数组中,数组所有的元素都初始化为'0',也就是空指针NULL
    // 用for循环,将除了 argv[0] 之外的参数转到line中
    for(int i = 1 ; i < argc ; i++){
        line[i - 1] = argv[i];
    }
    char buf[512];
    int n = 0;
    char c;
    while(read(0, &c, sizeof(char)) == sizeof(char)){
        if(c == '\n'){
            buf[n] = '\0';    // 加字符串结束符
            line[argc - 1] = buf;
            xargs(line);
            n = 0;
        }
        else
            buf[n++] = c;
    }
    if(n > 0){
        buf[n] = '\0';
        line[argc - 1] = buf;
        xargs(line);
    }
    exit(0);
}