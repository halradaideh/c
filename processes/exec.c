#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

/*
 * No code is executed after exec, exec replaces the current process with the new process.
 * I have used this future to handle processes order.
 * Written by : Hamdan Radaideh
*/

/*
    exec [l/v] [p/e]
        l -> argument are sent in the command explicitly                        [ REQUIRED / unless using v]
        v -> argument are sent in the command as array                          [ REQUIRED / unless using l]
        p -> look local PATH , ( by default you should send Full File Path )    [ OPTIONAL ]
        e -> array of pointers to environment arguments                         [ OPTIONAL ]
    ex:
        char *argv[] = { "ls", "-l", NULL };
        execvp(argv[0], argv);

        execl("/usr/bin/uname", "uname", "-r", NULL);

        char *const envp[2] = {"VAR=VALUE", NULL};
        execle("/u/userid/bin/newShell", "newShell", NULL, envp);

        execlp("uname", "uname", "-r", NULL);
*/

// colors defenitions
void red () {
  printf("\033[1;31m");
}

void green () {
  printf("\033[1;32m");
}

void yellow () {
  printf("\033[1;33m");
}

void blue () {
  printf("\033[0;34m");
}

void reset () {
  printf("\033[0m");
}

// function to print output for each process
void print_stuff(char c, char cmd[]){
	blue();
	printf("****************************************\n");
	green();
	printf("Process %c : \n",c);
	red();
	printf("\tProcess  PID: \t%d\n", getpid());
	yellow();
	printf("\tParent's PID: \t%d\n", getppid());
	green();
	printf("exec output for command : \n$ %s \n", cmd);
	reset();
	printf("\n");
}

int main( void ) {
	int pid = fork();
	if ( pid == 0 ) {
		char cmd[]="pwd";
		print_stuff('B', cmd);
		execlp("pwd", "pwd", NULL);
	}
	wait();

	int pid2 = fork();
	if ( pid2 == 0 ) {
		int pid3 = fork();
		if ( pid3 == 0 ) {
			char cmd[]="who";
			print_stuff('D', cmd);
		        execlp("who", "who", NULL);
		}
		wait();

		int pid4 = fork();
		if ( pid4 == 0 ) {
			char cmd[]="uname -r";
			print_stuff('E', cmd);
		        execlp("uname", "uname", "-r", NULL);
		}
		wait();

		char cmd[]="date";
		print_stuff('C', cmd);
		execlp("date", "date", NULL);
	}
	wait();

	char cmd[]="ls -la";
	print_stuff('A', cmd);
	char *argv[] = { "ls", "-l", 0 };
	execvp(argv[0], argv);
}
