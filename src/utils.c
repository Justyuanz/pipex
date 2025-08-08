#include "pipex.h"

void safe_exit(t_pipex *p, int exit_code, char *msg)
{
	if (msg)
		ft_putstr_fd(msg, 2);
	if (p->infile_fd > 0)
		close(p->infile_fd);
	if (p->outfile_fd > 0)
		close(p->outfile_fd);
	if (p->pipefd[0] > 0)
		close(p->pipefd[0]);
	if (p->pipefd[1] > 0)
		close(p->pipefd[1]);

	if (p->split_paths)
		free_split(p->split_paths);
	if (p->split_cmd)
		free_split(p->split_cmd);
	if (p->full_path)
		free(p->full_path);
	exit(exit_code);
}
void free_split(char **arr)
{
	int	i;

	i = 0;
	while(arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
}
