#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>


// Read from pipe
int read_from_pipe(int fd, void *buf, size_t bytes_left) {
	int has_read = 0;
	while (bytes_left > 0) {
		ssize_t nbytes = read(fd, buf, bytes_left);
       		if (nbytes == 0) {
        		if (has_read) {
                        	fprintf(stderr, "Read failed: Premature EOF");
                        	exit(EXIT_FAILURE);
                	}
               		return 0;
        	}
        	has_read = 1;
       		buf += nbytes;
        	bytes_left -= nbytes;
	}
	return 1;
}


// Write to pipe
void write_to_pipe(int fd, const void *buf, size_t bytes_left) {
	while (bytes_left > 0) {
		ssize_t nbytes = write(fd, buf, bytes_left);
		if (nbytes == -1) {
			exit(EXIT_FAILURE);
		}
		buf += nbytes;
		bytes_left -= nbytes;
	}
}


int main(void) {
	// Pipes
	// pipe1[0]: Parent Read
	// pipe1[1]: Child Write
	// pipe2[0]: Child Read
	// pipe2[1]: Parent Write 
	int pipe1[2];
	int pipe2[2];
	// PID of child process
	pid_t childpid;
	// Stores input integer from user
	int integer;
	// Stores integer read from pipe
	int readInteger;
	// Stores running sum of integers
	int integerSum = 0;
	// Stores sum of all integers read from pipe
	int readIntegerSum;

	pipe(pipe1);
	pipe(pipe2);

	if ((childpid = fork()) == -1) {
		perror("fork");
		exit(0);
	}

	if (childpid == 0) {
		// Close unused ends of pipe
		close(pipe1[0]);
		close(pipe2[1]);
		while(1) {
			// Get input from user
			scanf("%d", &integer);
			printf("Writing to pipe: %d \n", integer);
			// Write user input to pipe for parent process
			write_to_pipe(pipe1[1], &integer, sizeof(integer));
			// Check if user input is -1
			if (integer == -1)
				break;
		}
		read_from_pipe2(pipe2[0], &readIntegerSum, sizeof(readIntegerSum));
		printf("Sum of all integers: %d \n", readIntegerSum);
		exit(0);
	}

	else {
		// Close unused ends of pipe
		close(pipe1[1]);
		close(pipe2[0]);
		// Read integer from child process
		while(read_from_pipe2(pipe1[0], &readInteger, sizeof(readInteger))) {
			printf("Received from pipe: %d \n", readInteger);
			if (readInteger == -1) {
				// Write final integer sum to child process
				write_to_pipe(pipe2[1], &integerSum, sizeof(integerSum));
				break;
			}
			// Add integer to running sum
			integerSum += readInteger;
				
		}
	}

	return 0;
}
