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

    fprintf(stderr, "Process %d opened file %s for reading\n", mypid, argv[1]);
    int fd = open(argv[1], O_RDONLY); //open a file for reading
    if (fd < 0) {
	perror("open");
	exit(1);
    }
    kill(mypid,SIGSTOP);

    dup2(fd,0);
    fprintf(stderr, "Process %d called dup2(%d,0)\n", mypid, fd);
    kill(mypid, SIGSTOP);

    close(fd);
    fprintf(stderr, "Process %d called close(%d)\n", mypid, fd);
    kill(mypid, SIGSTOP);
    
    pid_t cpid = fork();
    if (cpid < 0) {
	perror("fork");
	exit(1);
    } else if (cpid == 0) {
	//child code
	mypid = getpid();
	fprintf(stderr, "Child %d started\n", mypid);
	kill(mypid,SIGSTOP);
	execl("./child", "child", NULL);
	perror("execl");
	_Exit(1);
    } else {
	//parent code
    }
    fprintf(stderr, "Process %d stopping before clean exit\n", mypid);
    kill(mypid, SIGSTOP);
    fprintf(stderr, "Process %d exiting cleanly\n", mypid);
    return EXIT_SUCCESS;
}
