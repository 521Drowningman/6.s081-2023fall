#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"


void find(char *path, char *name)
{
    char buf[512], *p;
    int fd;
    struct dirent de;   //表示目录条目
    struct stat st;
    
    if((fd = open(path, O_RDONLY)) < 0){
        fprintf(2, "find: %s: No such file or directory\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){  // return -1 for error
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    }

    if(st.type != T_DIR){
        fprintf(2, "find: %s: is not a directory\n", path);
        close(fd);
        return;
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf)){
        printf("find: path too long\n");
        return;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);  // p移动到buf的末尾,strlen(buf)的长度和path相等
    *p++ = '/';  // 覆盖了字符串结束符'\0'
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0){   // inode为0说明该目录是空的或无效的
            continue;
        }
        if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0){   // 跳过. 和 .. 目录
            continue;
        }
        memmove(p, de.name, DIRSIZ);   // 将de.name复制到buf的末尾,如果DIRSIZ大于de.name那么会在buf后面补充额外的字节
        p[DIRSIZ] = 0;     // 补上\0,这个字符串结束标志
        if(stat(buf, &st) < 0){    //取出该文件或目录的详细信息
            printf("find: cannot stat %s\n", buf);
            continue;
        }
        if(st.type == T_DIR){
            // 如果是目录则要递归继续去寻找
            find(buf, name);
        }
        else{
            if(strcmp(de.name, name) == 0){
                printf("%s\n", buf);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3){
        fprintf(2 , "Usage:find <directory> <filename>\n");
        exit(1);
    }
    find(argv[1] , argv[2]);
    exit(0);
}