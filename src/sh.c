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


#include <sh.h>

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



