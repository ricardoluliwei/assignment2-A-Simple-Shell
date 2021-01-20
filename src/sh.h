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
	int job_id;
	pid_t pid;
	char command_line[80];
	int status;
};



int compare(int a, int b);
char *sh_read_line(void);
int execute(char **args);

// build in command
void run_jobs();
void run_fg(int id_type, int id);
void run_bg(int id_type, int id);
void run_kill(int id_type, int id);
void quit();

// IO redirection
void change_output(char *file_name);
void change_input(char *file_name);

// Signal handler
void int_handler(int sig);  // terminate
void stop_handler(int sig); // stop
void cont_handler(int sig); // continue

#endif