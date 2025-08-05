#include "pipex.h"
// make cmd1 read from infile instead of stdin, and write to pipe instead of stdout
//You're not closing unused pipe ends

void	handle_child(t_pipex *p, int index)
{
	int		i;

	i = 0;
	while (p->envp[i])
	{
		if (ft_strncmp(p->envp[i], "PATH=", 5) == 0)
			break;
		i++;
	}
	if (!p->envp[i])
		safe_exit(p, 1, "PATH not found");
	p->split_paths = ft_split(p->envp[i] + 5, ':');
	if (!p->split_paths)
		safe_exit(p, 1, "split fail");
	p->split_cmd = split_command(p, index);//what if it fails
	if (!p->split_cmd)
		safe_exit(p, 1, "split fail");
	find_right_path(p, index);
	free_split(p->split_cmd);
	free_split(p->split_paths);
}

pid_t subprocess(t_pipex *p, int index)
{
	pid_t	id;

	id = fork();
	if (id == -1) //HANDLE ERR
	{
		perror("fork");
		safe_exit(p, 1, NULL);
	}
	if (id == 0)
		handle_child(p, index);
	return id;
}
int	main(int argc, char **argv, char **envp)
{
	t_pipex	p;

	if (argc != 5)
		write (2, "Usage: ./pipex infile \"cm1\" \"cm2\" outfile\n", 42);
	p.argv = argv;
	p.envp = envp;
	p.infile_fd = -1;
	p.outfile_fd = -1;
	p.pipefd[0] = -1;
	p.pipefd[1] = -1;
	if (pipe(p.pipefd) == -1) //HANDLE ERR
	{
		perror("pipe");
		safe_exit(&p, 1, NULL);
	}
	p.pid[0] = subprocess(&p, 0); //first child process
	p.pid[1] = subprocess(&p, 1); //second child process
}
