#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// user-defined signal handler for SIGALRM
void sigalrm_handler(int signo) {
	if (signo == SIGALRM) {
		printf("Alarm\n");
	}
}

// user-defined signal handler for SIGINT
void sigint_handler(int signo) {
	if (signo == SIGINT) {
		printf(" CTRL+C pressed!\n");
	}
}

// user-defined signal handler for SIGTSTP
void sigtstp_handler(int signo) {
	if (signo == SIGTSTP) {
		printf(" CTRL+Z pressed!\n");
		exit(1);
	}
}

int main(void) {
	// register the SIGALRM handler
	if (signal(SIGALRM, sigalrm_handler) == SIG_ERR) {
		printf("Failed to register alarm handler.");
		exit(1);
	}

	// register the SIGINT handler
	if (signal(SIGINT, sigint_handler) == SIG_ERR) {
		printf("Failed to register CTRL C handler.");
		exit(1);
	}

	// register the SIGTSTP handler
	if (signal(SIGTSTP, sigtstp_handler) == SIG_ERR) {
		printf("Failed to register CTRL Z handler.");
		exit(1);
	}

	while(1) {
		sleep(2);
		raise(SIGALRM);
	}
}



