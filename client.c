#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <unistd.h>
#include <fcntl.h>

#include "key.h"

int main(){
	struct sembuf down = {0 , -1, SEM_UNDO};
	struct sembuf up = {0, 1, SEM_UNDO};

	int semd = semget(KEY, 0, 0644);
	int shmd = shmget(KEY, 0, 0644);
	
	semop(semd, &down, 1);

	//print prev line
	char * prev_line = (char *) shmat(shmd, 0, 0);
	printf("Previous Line: %s\n", prev_line);
	
	//prompt new line
	char new_line [BUFSIZ];
	printf("Next Line: ");
	fgets(new_line, BUFSIZ, stdin);
	
	//overwrite
	strncpy(prev_line, new_line, BUFSIZ);

	//write to file
	int fd = open("story.txt", O_WRONLY | O_APPEND);
	write(fd, new_line, strlen(new_line));
	close(fd);

	shmdt(prev_line);
	semop(semd, &up, 1);

	return 0;
}

