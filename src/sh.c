#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>
#include <fcntl.h>

#include "sh.h"

//Constants
#define Maxline 80
#define MaxArgc 80
#define Maxjob 5

// Job status
#define EMPTY 0
#define RUNNING 1
#define FOREGROUND 2
#define STOPPED 3

//build-in commands
#define JOBS "jobs"
#define BG "bg"
#define FG "fg"
#define KILL "kill"
#define QUIT "quit"

// Sign
#define Ampersand '&'
#define Percentage_sign '%'

#define MAX_LINE 100
#define SH_TOK_DELIM " \t\r\n\a"

struct Job jobs[Maxjob];

int compare(int a, int b){
	if(a == 0||a>b){
		return b;
	}else{
		return a;
	}
}

// build in command

void print_jobs(){
	int i;
	for(i=0; i<Maxjob; i++){
		if(jobs[i].status == RUNNING)
			printf("[%d] (%d) Running %s\n", i, jobs[i].pid, jobs[i].command_line);
		else if(jobs[i].status == STOPPED)
			printf("[%d] (%d) Stopped %s\n", i, jobs[i].pid, jobs[i].command_line);
		else if(jobs[i].status == FOREGROUND)
			printf("[%d] (%d) Foreground %s\n", i, jobs[i].pid, jobs[i].command_line);
	}
}

void run_fg(char** args){
	int i;
	
	for(i = 0; i < Maxjob; i++){
		if(jobs[i].status == FOREGROUND)
			kill(jobs[i].pid, SIGTSTP);
	}

	if(args[1][0] == '%'){
		kill(jobs[atoi(args[1])].pid, SIGCONT);
		//change stopped process to fg by JID
	}else{
		kill(atoi(args[1]), SIGCONT);
		//change stopped process to fg by PID
	}

	kill(getpid(), SIGCHLD);
}

void run_bg(char** args){
	if(args[1][0] == '%'){
		kill(jobs[atoi(args[1])].pid, SIGSTOP);
		//place a running process to bg by JID
	}else{
		kill(atoi(args[1]), SIGSTOP);
		//place a running process to bg by PID
	}	
}

void run_kill(char** args){
	if(args[1][0] == Percentage_sign){
		//kill process to bg by JID
		kill(jobs[atoi(args[1])].pid, SIGINT);
	}else{
		//kill a running process to bg by PID
		kill(atoi(args[1]), SIGINT);
	}
}

void int_handler(int sig){
    printf("Process %d received signal %d\n", getpid(), sig);
    exit(0);
}

void stop_handler(int sig){
    pause();
}

void child_handler(int sig){
    pid_t pid;
	int status;
	int i;

	while((pid = waitpid(-1, &status, WUNTRACED)) >= 0){
		// find the child that sent the signal
		for(i=0; i < Maxjob; i++){
			if(jobs[i].pid == pid){
				if(WIFEXITED(status)) // if exit, clear the space
					memset(&jobs[i], 0, sizeof(struct Job));
				else if(WIFSTOPPED(status)) // if stopped, change its status to stop
					jobs[i].status = STOPPED;
				
				break;
			}
		}
	}

	// check is there any foreground process
	for(i = 0; i < Maxjob; i++){
		if(jobs[i].status == FOREGROUND)
			pause();
	}

}


char *sh_read_line(void)
{
	char *line = NULL;
	size_t bufsize = 0;  // have getline allocate a buffer for us

	if (getline(&line, &bufsize, stdin) == -1) {
		if (feof(stdin))  // EOF
		{
			fprintf(stderr, "EOF\n");
			exit(EXIT_SUCCESS);
		} else {
			//fprintf(stderr, "Value of errno: %d\n", errno);
			exit(EXIT_FAILURE);
		}
	}
	return line;
}


int execute(char **args ){
    int i;
    int opos = 0;//check the pos of >
	int ipos = 0;//check the pos of <
	int ofound = 0;
	int ifound = 0;
	int pfound = 0;
	int isBackgroundTask = 0;
    pid_t pid;
    int status;
	int jobID = -1;

    for(i = 0; i <80; i++){
        if( args[i] != NULL){
			if(args[i][0] ==  Ampersand)
				isBackgroundTask = 1;
        }
    }
	//check ouput to file
	
	while(1){
		if(args[opos] != NULL){
			if(args[opos][0] == '>'){
				ofound = 1;
				break;
			}
		}else{
			break;
		}
		++opos;
	}

	//check input from file
	while(1){
		if(args[ipos] != NULL){
			if(args[ipos][0] == '<'){
				ifound = 1;
				break;
			}				
		}else{
			break;
		}
		++ipos;
	}

	// check if the Job reaches max
	for(i = 0; i < Maxjob; i++){
		if(jobs[i].status == EMPTY){
			jobID = i;
			break;
		}
	}

	if(jobID == -1){
		printf("No space to execute a job!\n");
		return -1;
	}

    pid = fork();
	if(pid == 0){
		// child process
        mode_t mode = S_IRWXU | S_IRWXG | S_IRWXO;
		if(ofound == 1||ifound == 1){ 
			if(ifound==1){//input from file
				int fi = open(args[ipos+1], O_RDONLY, mode);
				dup2(fi, STDIN_FILENO);
				close(fi); 
			}
			if(ofound==1){// output to  file
				int fo = open(args[opos+1], O_CREAT|O_WRONLY|O_TRUNC, mode);
				dup2(fo, STDOUT_FILENO);
				close(fo);
			}
			char *buffer[100];
			
			for(i=0; i<compare(ipos, opos); i++){
					buffer[i] = args[i];
					}
			if(args[0][0] == '/'){
				if(execv(args[0], args) <0){
					perror("Error!");
					exit(0);
					}
			}else{
				if(execvp(args[0], args) <0){
					perror("Error!");
					exit(0);
					}
				}
			
		}else{
			
			if(args[0][0] == '/'){
				if(execv(args[0], args) <0){
					perror("Error!");
					exit(0);
					}
			}else{
				if(execvp(args[0], args) <0){
					perror("Error!");
					exit(0);
					}
				}
		}	
	} else{
		jobs[jobID].pid = pid;
		if(isBackgroundTask){
			jobs[jobID].status = RUNNING;
		} else {
			jobs[jobID].status = FOREGROUND;
		}
	}

	return jobID;
}


int main(){
    char input[800], *buffer, **args_buf, full[800];
	int bufsize = MAX_LINE;
	char **args = malloc(bufsize *sizeof(char *));
    int counter = 0, i, j;
	
    //for(i =0; i< 80;i++){
            //strcpy(args[i], "");}

	/* Install the handler */
	if (signal(SIGINT, int_handler) == SIG_ERR)
		perror("signal error");

	if (signal(SIGTSTP, stop_handler) == SIG_ERR)
		perror("signal error");

	if (signal(SIGCHLD, child_handler) == SIG_ERR)
		perror("signal error");	

    while (1) // while loop to get user input
    {
        printf("prompt> ");
		memset(input, 0, 80);
		memset(args, 0, bufsize *sizeof(char *));
        fgets(input, (sizeof input / sizeof input[0]), stdin);
        if(input[strlen(input)-1] == '\n') input[strlen(input)-1]=0;
		
        if(strcmp(input, "quit") == 0){break;}
        strcpy(full, input);
        buffer = strtok(input, " ");

		counter = 0;
        while(buffer) {
            //printf( "%s\n", buffer );
            //strcpy(args[counter], buffer); 
			args[counter] = buffer;
			
			if(counter>= bufsize){
				bufsize += MAX_LINE;
				args_buf = args;
				args = realloc(args, bufsize *sizeof(char *));
				if(!args){
					free(args_buf);
					fprintf(stderr, "sh: allocation error\n");
					exit(EXIT_FAILURE);
				}
			}
			
			buffer = strtok(NULL, " ");
            counter++;
        }

		// build in command
		if(strcmp(args[0], FG)==0){
			run_fg(args);
		} else if(strcmp(args[0], BG) == 0){
			run_bg(args);
		} else if(strcmp(args[0], KILL) == 0){
			run_kill(args);
		} else if(strcmp(args[0], QUIT) == 0){
			exit(0);
		} else if(strcmp(args[0], JOBS) == 0){
			print_jobs();
		} else {
		//general case
        int jobID = execute(args);
		if(jobID != -1){
			strcpy(jobs[jobID].command_line, input);
			if(jobs[jobID].status == FOREGROUND)
				pause();
			}
		}
    }
    
    return 0;
}
