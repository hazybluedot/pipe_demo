#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <signal.h>

#define BUF_SIZE 4096

int main(int argc, char *argv[]) {
    char buffer[BUF_SIZE];
    char out_buffer[BUF_SIZE*2];
    size_t nbytes = 0;
    ssize_t bytes_read;
    pid_t mypid = getpid();

    fprintf(stderr, "Parent %d about to call pipe()\n", mypid);
    int pfd[2];
    if (pipe(pfd) < 0) {
	perror("pipe");
	exit(1);
    }

    fprintf(stderr, "Parent %d about to call fork()\n", mypid);
    pid_t cpid = fork();
    if (cpid < 0) {
	perror("fork");
	exit(1);
    } else if (cpid == 0) {
	//child code
	mypid = getpid();
	fprintf(stderr, "Child %d started\n", mypid);

	close(pfd[1]); //unused end
	dup2(pfd[0],0);
	fprintf(stderr, "Process %d called close(%d) and dup2(%d,0)\n", mypid, pfd[1], pfd[0]);

	close(pfd[0]);
	fprintf(stderr, "Process %d called close(%d)\n", mypid, pfd[0]);
    
	execl("./child", "child", NULL);
	perror("execl");
	_Exit(1);
    } else {
	//parent code
	close(pfd[0]); //close unused end
	nbytes = sizeof(buffer);
	while(bytes_read = read(0, buffer, nbytes)) { //read from standard in (FD 0)
	    if (bytes_read == EOF) break;
	    buffer[bytes_read] = 0;
	    sprintf(out_buffer, "Retrieved %zu bytes from %d:\n%s", bytes_read, mypid, buffer);
	    write(pfd[1], out_buffer, strlen(out_buffer)); //write to standard out (FD 1)
	}
	close(pfd[1]);
    }
    kill(mypid,SIGSTOP);
    wait();
    fprintf(stderr, "Process %d stopping before clean exit\n", mypid);
    kill(mypid, SIGSTOP);
    fprintf(stderr, "Process %d exiting cleanly\n", mypid);
    return EXIT_SUCCESS;
}
