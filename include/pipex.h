#ifndef PIPEX_H
# define PIPEX_H

#include "../libft/libft.h"
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h> //DELETE ME

typedef struct s_pipex{
	char	**argv;
	char	**envp;
	int		pipefd[2];
	pid_t	pid[2];
}	t_pipex;

void free_split(char **arr);

#endif