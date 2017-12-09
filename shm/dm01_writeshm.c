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

#define ERR_EXIT(m) \
		do \
		{ \
				perror(m); \ 
				exit(EXIT_FAILURE); \
		} while(0)

typedef struct _Teacher{
	char name[64];
	int age;
} Teacher;

int main(){
	int ret = 0;
	int shmid;
	shmid = shmget(0x2232, sizeof(Teacher), 0666 | IPC_CREAT);
	if(shmid == -1){
		perror("shmget err ..2222.");
		if(shmid < 0 && errno==ENOENT){
			printf("check share memory not exit\n");
		}
		printf("shmid errno: %d\n", shmid);
		return errno;
	}
	Teacher *p = NULL;
	p = shmat(shmid, NULL, 0);
	if(p == (void *)-1){
		perror("shmget err");
		return errno;
	}

	strcpy(p->name, "aaaa");
	p->age = 33;

	printf("shut down the share memory, other not delete \n");

	int num;
	scanf("%d", &num);

	if(num == 1){
		// drop the link to share memory
		shmdt(p);
		ret = shmctl(shmid, IPC_RMID, NULL);
		if(ret < 0){
			perror("rm errr\n");
		}
	}
	return 0;
}