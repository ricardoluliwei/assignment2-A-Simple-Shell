#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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
int execute(char args[80][80] ){
    int i;
    for(i = 0; i <80; i++){
        if(strcmp(args[i], "") != 0){
            printf("%s\n", args[i]);
        }
    }
    return 1;

}

int main(){
    char input[80], *buffer, full[80];
    char args[80][80];
    
    int counter = 0, i, j;
    for(i =0; i< 80;i++){
            strcpy(args[i], "");
    }
    
    while (1)
    {
        printf("prompt> ");
        fgets(input, (sizeof input / sizeof input[0]), stdin);
        if(input[strlen(input)-1] == '\n') input[strlen(input)-1]=0; 
        if(strcmp(input, "quit") == 0){break;}
        strcpy(full, input);
        buffer = strtok(input, " ");
        while( buffer ) {
            //printf( "%s\n", buffer );
            strcpy(args[counter], buffer);
            buffer = strtok(NULL, " ");
            counter++;
        }
        execute(args);
        for(i =0; i< 80;i++){
            strcpy(args[i], "");
        }
        counter = 0;
    }
    
   
    return 0;
}