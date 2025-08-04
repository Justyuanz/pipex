#ifndef PIPEX_H
# define PIPEX_H

#include "../libft/libft.h"
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h> //DELETE ME

typedef struct s_pipex{
	char	**argv;
	char	**envp;
	int		infile_fd;
	int		outfile_fd;
	int		pipefd[2];
	pid_t	pid[2];
}	t_pipex;

void free_split(char **arr);
void p_error(int exit_code, char *func);
#endif