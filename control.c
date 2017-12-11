#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <wait.h>

#include "key.h"


void create(){
	int semd = semget(KEY, 1, IPC_CREAT | IPC_EXCL | 0644);
	int shmd = shmget(KEY, BUFSIZ, IPC_CREAT | IPC_EXCL | 0644);
	int fd = open("story.txt", O_CREAT | O_EXCL | O_TRUNC, 0644 );
	close(fd);
	semctl(semd, 0, SETVAL, 1);

}

void view(){
	int fd = open("story.txt", O_RDONLY, 0);
	
	int f = fork();
	
	if (f == 0){ 
		execlp("cat", "cat", "story.txt", 0);
	}
	else{
		int status;
		wait(&status);
	}
}

void rem(){
	int semd = semget(KEY,0,0644);
	semctl(semd, 0, IPC_RMID);

	int shmd = shmget(KEY,0,0644);
	shmctl(shmd, IPC_RMID, 0);
	
	view();
	remove("story.txt");
}

void usage(){
	printf("\nUSAGE ./control <-c | -v | -r>\n\n");
	exit(1);
}


int main(int argc, char ** argv){
	if (argc != 2){
		usage();
	}
		

	if ( !strcmp(argv[1], "-c") ){
		create();
		printf("Story Created\n");
	}
	
	else if ( !strcmp(argv[1], "-v") ){
		view();
	}

	else if ( !strcmp(argv[1], "-r") ){
		rem();
		printf("\nStory Removed\n");
	}

	else {
		usage();
	}
}
