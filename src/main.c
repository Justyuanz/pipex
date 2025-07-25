#include "pipex.h"
int handle_files(char *filename)
{
	int fd;

	fd = open(filename, O_RDONLY);
	if (fd  == -1)
		exit (1);
	return (fd);
}
void find_right_path(char **split_path, char **split_cmd, t_pipex *p)
{
	int		i;
	int		infile_fd;
	char	*tmp;
	char	*full_path;

	i = 0;
	while (split_path[i])
	{
		tmp = ft_strjoin(split_path[i], "/");
		full_path = ft_strjoin(tmp, split_cmd[0]); //free
		free(tmp);
		if (access(full_path, X_OK) == 0)
		{
			infile_fd = handle_files(p->argv[1]);
			if (execve(full_path, split_cmd, p->envp) == -1)
			{
				free(full_path);
				return;
			}
		}
		free(full_path);
		i++;
	}
}
void	handle_child(t_pipex *p)
{
	int		i;
	char 	**split_paths;
	char	**split_cmd;

	i = 0;
	while (p->envp[i])
	{
		if (ft_strncmp(p->envp[i], "PATH=", 5) == 0)
			break;
		i++;
	}
	split_paths = ft_split(p->envp[i] + 5, ':'); //free
	split_cmd = ft_split(p->argv[2], ' '); //free
	find_right_path(split_paths, split_cmd, p);
	free_split(split_cmd);
	free_split(split_paths);
}
pid_t subprocess(t_pipex *p)
{
	pid_t	id;

	id = fork();
	if (id == -1)
		exit (1);
	if (id == 0)
		handle_child(p);
	return id;
}
int	main(int argc, char **argv, char **envp)
{
	t_pipex	p;

	if (argc != 5)
	{
		write (2, "Usage: ./pipex infile \"cm1\" \"cm2\" outfile\n", 6);
		return (1);
	}
	p.argv = argv;
	p.envp = envp;
	if (pipe(p.pipefd) == -1)
		return (1);
	p.pid[0] = subprocess(&p);
}
