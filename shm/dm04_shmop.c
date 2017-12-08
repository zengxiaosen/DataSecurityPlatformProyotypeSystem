#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <sys/ipc.h>
#include <sys/shm.h>

void main41(){
	int shmid;
	shmid = shmget(0x3334, 128, 0666);
	if(shmid < 0){
		printf("get share memory error! \n");
		return;
	}else{
		printf("get share memory succeed! \n");
	}
	


	printf("hello\n");
}

void main(){
	int shmid;
	shmid = shmget(0x3334, 128, 0666 | IPC_CREAT);
	if(shmid < 0){
		printf("get share memory error! \n");
		return;
	}else{
		printf("get share memory succeed! \n");
	}
	


	printf("hello\n");
}