#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

int main (int argc, char *argv[]) {

	for(int i = 1; i < 4; i++) {
		for(int j = 1; j <= i; j++) {
			printf("%d | %d/%d = %d | %d\n", ((i/j) * j) != j  , i, j, i/j, i/j * j);
		}
	}

	// pid_t pid;
 //    int segment_id;
 //    int *shared_memory;
 //    const int size = sizeof(int);
    
	// /* allocate shared memory segment */   
 //    segment_id = shmget(IPC_PRIVATE, size, S_IRUSR | S_IWUSR);
    
	// /* attach the shared memory segment */   
	// //shared_memory = (char *) shmat(segment_id, NULL, 0);
	
	// pid = fork();
	// if (pid < 0)
	// {
	// 	fprintf(stderr, "Fork failed");
	// 	return 1;
	// }
	
	// if (pid > 0)
	// {
	// 	/* attach the shared memory segment */   
	// 	shared_memory = (int *) shmat(segment_id, NULL, 0);
    
	// 	printf("P1 is going to write: \n");
		
	// 	 write a message to the shared memory segment    
	// 	*shared_memory = 5;
		
	// 	/* detach the shared memory segment */    
	// 	shmdt(shared_memory);
		
	// 	sleep(5);
	// 	/* remove the shared memory segment */   
	// 	shmctl(segment_id, IPC_RMID, NULL);
	// }
	// else
	// {
	// 	sleep(3);
		
	// 	/* attach the shared memory segment */   
	// 	shared_memory = (int *) shmat(segment_id, NULL, 0);
		
	// 	printf("P2 is going to read: \n");
		
	// 	/* now print out the string from shared memory */   
	// 	printf("----- %d\n", *shared_memory);
		
	// 	/* detach the shared memory segment */    
	// 	shmdt(shared_memory);
	// }
	// return 0;
}
