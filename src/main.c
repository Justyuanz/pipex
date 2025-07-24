#include "pipex.h"
void	handle_child(t_pipex *p)
{
	int		i;
	int	j = 0;
	char 	**split_paths;
	char	**split_cmd;
	char	*path;
	i = 0;
	while (p->envp[i])
	{
		if (ft_strncmp(p->envp[i], "PATH=", 5) == 0)
			break;
		i++;
	}
	split_paths = ft_split(p->envp[i] + 5, ':');
	split_cmd = ft_split(p->argv[2], ' ');
	while (split_paths[j])
	{
		path = ft_strjoin(split_paths[j], )
		if (access(paths, ))
	}
	free_split(paths);
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
