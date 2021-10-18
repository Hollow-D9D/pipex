#ifndef PIPEX_H
#define PIPEX_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/errno.h>

typedef struct s_command
{
	char* name;
	char** args;
}	t_command;

typedef struct s_general
{
	int count;
	int** pipes;
	t_command* commands;
	int in_out[2];
	int infile;
	int outfile;
	int error_code;
	char **path;
	char *pwd;
}       t_general;

#endif
