#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define SEM_VAL 4
#define KEY 23545 
#define KEY2 123456

int create_sem(int count){
	int semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
	int shmd = shmget(KEY2, BUFSIZ, IPC_CREAT | IPC_EXCL | 0644);
	int fd = open("story.txt", O_CREAT | O_EXCL | O_TRUNC, 0644 );
	close(fd);
	semctl(semd, 0, SETVAL, count);
	return semd;
}

void view_sem(){
	int fd = open("story.txt", O_RDONLY, 0);
	char buf [SSIZE_MAX]; 
	read(fd, buf, SSIZE_MAX);
	printf("%s\n", buf);
}

int rem_sem(){
	int semd = semget(KEY,0,0);
	return semctl(semd, 0, IPC_RMID);
}

void usage(){
	printf("USAGE ./control <-c N | -v | -r>");
	exit(1);
}


int main(int argc, char ** argv){
	//if (argc != 2){
	//	usage();
	//}
	printf("%s\n", argv[1]);	

	int ret;
	
	if ( !strcmp(argv[1], "-c") ){
		int value = atoi(argv[2]);
		ret = create_sem( value );
		printf("semaphore created: %d\nvalue set: %d\n", ret, value);
	}
	
	else if ( !strcmp(argv[1], "-v") ){
		ret = view_sem();
		printf("semaphore value: %d\n", ret);
		
	}

	else if ( !strcmp(argv[1], "-r") ){
		ret = rem_sem();
		printf("semaphore removed: %d\n", ret);
	}

	else {
		usage();
	}
}
