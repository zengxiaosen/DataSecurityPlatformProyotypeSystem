#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "keymnglog.h"
#include "keymng_shmop.h"
#include "myipc_shm.h"

//先检验共享内存是否存在
//1 若存在则使用旧的
//2 若不存在，则创建

int KeyMng_ShmInit(int key, int maxnodenum, int *shmhdl){
    int ret = 0;
    return 0;
}