#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
// 
#include "util.c"
#include "../inc/processos.h"
//
void doWork(char *arg) {
    while (1) {
		printf("%s\n", arg);
	}
}

int main()
{
    /*Spawn a new process to run alongside us.*/
    pid_t pid = fork();
    if (pid == 0) {		/* child process */
		doWork("child");  
		exit(0);
    }
    else {			/* pid!=0; parent process */
		//printf("sou o pai e vou acabar logo");
		doWork("parent");
		waitpid(pid,0,0);	/* wait for child to exit */
    }
    return 0;
}

int main() {

}