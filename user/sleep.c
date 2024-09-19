#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    if(argc < 2){ // argc统计参数的数量(包括程序名本身),所以argc的大小至少是1
        fprintf(2, "sleep:no argument\n");
        exit(1); // 1指错误
    }
    int ret = sleep(atoi(argv[1]));  //argv[1]是传入的参数,atoi会将字符串转为数字,sleep will return num to imply success or fail
    exit(ret);
}