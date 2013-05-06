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

    //Main IO loop 
    nbytes = sizeof(buffer);
    while(bytes_read = read(0, buffer, nbytes)) { //read from standard in (FD 0)
	if (bytes_read == EOF) break;
	buffer[bytes_read] = 0;
	sprintf(out_buffer, "Retrieved %zu bytes:\n%s", bytes_read, buffer);
	write(1, out_buffer, strlen(out_buffer)); //write to standard out (FD 1)
    }
    //End Main IO loop
    fprintf(stderr, "Process %d exiting cleanly\n", mypid);
    return EXIT_SUCCESS;
}
