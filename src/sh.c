#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>
#include <fcntl.h>

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
			if(execvp(args[0], buffer) <0){
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
		wait(&childpid);
		return 1;
	}
	
	return 0;
    }
    

#define TOKEN_SIZE 80
	#define SH_TOK_DELIM " \t\r\n\a"

int main(){
    char input[80], *buffer, **args_buf, full[80];
	int bufsize = TOKEN_SIZE;
	char **args = malloc(bufsize *sizeof(char *));

    
    int counter = 0, i, j;
	
    //for(i =0; i< 80;i++){
            //strcpy(args[i], "");}

    while (1)
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
        execute(args);
		/*
        for(i =0; i< 80;i++){
            strcpy(args[i], "");
        }
		*/
        counter = 0;
    }
    
    return 0;
}