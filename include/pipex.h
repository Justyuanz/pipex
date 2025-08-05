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
	int		infile_fd;
	int		outfile_fd;
	int		pipefd[2];
	pid_t	pid[2];

	char	**argv;
	char	**envp;
	char 	**split_paths;
	char	**split_cmd;
	char	*full_path;
}	t_pipex;

void free_split(char **arr);
void safe_exit(t_pipex *p, int exit_code, char *msg);
void find_right_path(t_pipex *p, int index);
void handle_child(t_pipex *p, int index);
char	**split_command(t_pipex *p, int index);

#endif