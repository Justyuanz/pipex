#include "pipex.h"
void redirect_fds(t_pipex *p, int index)
{
	// make cmd1 read from infile instead of stdin, and write to pipe instead of stdout
	if (index == 0)
	{
		dup2(p->infile_fd, 0); //HANDLE FAIL
		dup2(p->pipefd[1], 1); //HANDLE FAIL
	}
	if (index == 1)
	{
		dup2(p->pipefd[0], 0);
		dup2(p->outfile_fd, 1);
	}
}
int handle_files(int index, t_pipex *p)
{
	int fd;

	if (index == 0)
		fd = open(p->argv[1], O_RDONLY);
	else
		fd = open(p->argv[4], O_RDWR | O_CREAT | O_TRUNC);
	if (fd  == -1) //HANDLE ERR
	{
		perror("open");
		safe_exit(p, 1, NULL);
	}
	return (fd);
}
void run_command(t_pipex *p,int index)
{
	if (access(p->full_path, X_OK) == 0) //WHAT IF IT FAIL, WHAT MSG TO PUT?
	{
		if (index == 0)
			p->infile_fd = handle_files(index, p);
		else if (index == 1)
			p->outfile_fd = handle_files(index, p);
		redirect_fds(p, index); //HANDLE ERR
		if (execve(p->full_path, p->split_cmd, p->envp) == -1) //HANDLE ERR
		{
			free(p->full_path);
			perror("execve");
			safe_exit(p, 127, NULL);
		}
	}
}
void	find_right_path(t_pipex *p, int index)
{
	int		i;
	char	*tmp;

	i = 0;
	while (p->split_paths[i])
	{
		tmp = ft_strjoin(p->split_paths[i], "/"); //WHAT IF IT IS NULL
		if (!tmp)
			safe_exit(p, 1, "strjoin fail");
		p->full_path = ft_strjoin(tmp, p->split_cmd[0]); //free, WHAT IF IT IS NULL
		if (!p->full_path)
		{
			free(tmp);
			safe_exit(p, 1, "strjoin fail");
		}
		free(tmp);
		run_command(p, index); // what if it fails
		free(p->full_path);
		i++;
	}
	if (access(p->full_path, X_OK) == -1)
	{
		if (errno == EACCES)
		{
			perror("access");
			safe_exit(p, 126, NULL);
		}
		else
			safe_exit(p, 127, "command not found");
	}
}
char	**split_command(t_pipex *p, int index)
{
	char	**cmd;

	if (index == 0)
	{
		cmd = ft_split(p->argv[2], ' ');
		if (!cmd)
			return (NULL);
	}
	else
	{
		cmd = ft_split(p->argv[3], ' ');
		if (!cmd)
			return (NULL);
	}
	return (cmd);
}

