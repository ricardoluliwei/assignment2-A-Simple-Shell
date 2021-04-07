#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <signal.h>
#include <fcntl.h>

#ifndef SH_H
#define SH_H

struct Job{
	pid_t pid;
	char command_line[80];
	int status; // 0 for empty, 1 for Running, 2 for Foreground, 3 for Stopped
};



int compare(int a, int b);
char *sh_read_line(void);
int execute(char **args);

// build in command
void print_jobs();
void run_fg(char** args);
void run_bg(char** args);
void run_kill(char** args);

// IO redirection
void change_output(char *file_name);
void change_input(char *file_name);

// Signal handler
void int_handler(int sig);  // terminate
void stop_handler(int sig); // stop

void child_handler(int sig);

#endif