#ifndef SH_H
#define SH_H

int compare(int a, int b);
char *sh_read_line(void);
int execute(char **args);

// build in command
void run_jobs();
void run_fg(char *args);
void run_bg(char *args);
void run_kill(char *args);
void quit();

// IO redirection
void change_output(char *file_name);
void change_input(char *file_name);

#endif