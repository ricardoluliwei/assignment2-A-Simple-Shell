#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
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

//build-in commands
#define JOBS "jobs"
#define BG "bg"
#define FG "fg"
#define KILL "kill"
#define QUIT "quit"

// Sign
#define Ampersand "&"
#define Percentage_sign "%"

#define TOKEN_SIZE 80
#define SH_TOK_DELIM " \t\r\n\a"

struct Job jobs[Maxjob];

int compare(int a, int b){
	if(a == 0||a>b){
		return b;
	}else{
		return a;
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
    pid_t pid;
    int childpid;
	
    for(i = 0; i <80; i++){
        if( args[i] != NULL){
            printf("%s\n", args[i]);
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
    pid = fork();
	if(pid == 0){
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
	}else{
		wait(&childpid);
		return pid;
	}
	
	return 0;
}


int main(){
    char input[80], *buffer, **args_buf, full[80];
	int bufsize = TOKEN_SIZE;
	char **args = malloc(bufsize *sizeof(char *));
    int counter = 0, i, j;
	
    //for(i =0; i< 80;i++){
            //strcpy(args[i], "");}

    while (1) // while loop to get user input
    {
        printf("prompt> ");
        fgets(input, (sizeof input / sizeof input[0]), stdin);
        if(input[strlen(input)-1] == '\n') input[strlen(input)-1]=0;
		
        if(strcmp(input, "quit") == 0){break;}
        strcpy(full, input);
        buffer = strtok(input, " ");
        while(buffer) {
            //printf( "%s\n", buffer );
            //strcpy(args[counter], buffer); 
			args[counter] = buffer;
			if(counter>= bufsize){
				bufsize += TOKEN_SIZE;
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
		if(strcmp(args[0], "fg")==0){
			if(args[0][0] == '%'){
				//change stopped process to fg by JID
			}else{
				//change stopped process to fg by PID
			}

		}
		if(strcmp(args[0], "bg")==0){
			if(args[0][0] == '%'){
				//place a running process to bg by JID
			}else{
				//place a running process to bg by PID
			}

		}
		if(strcmp(args[0], "kill")==0){
			if(args[0][0] == '%'){
				//kill process to bg by JID
			}else{
				//kill a running process to bg by PID
			}

		}
		//general case
        pid_t pid = execute(args);
		jobs[0].pid = pid;
		strcpy(jobs[0].command_line, input);
		jobs[0].status = 1;
        counter = 0;
    }
    
    return 0;
}
