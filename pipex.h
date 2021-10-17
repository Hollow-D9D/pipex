#ifndef PIPEX_H
#define PIPEX_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_command
{
	char* name;
	char** args;
}	t_command;

typedef struct s_general
{
	int* pipes[2];
	t_command* commands;
	int in_out[2];
	int infile;
	int outfile;
	int error_code;
	char **path;
}       t_general;

#endif
