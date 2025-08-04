#include "pipex.h"
// make cmd1 read from infile instead of stdin, and write to pipe instead of stdout
//You're not closing unused pipe ends
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
		exit (1);
	return (fd);
}
char *find_right_path(char **split_path, char **split_cmd, t_pipex *p, int index)
{
	int		i;
	char	*tmp;
	char	*full_path;

	i = 0;
	while (split_path[i])
	{
		tmp = ft_strjoin(split_path[i], "/"); //WHAT IF IT IS NULL
		if (!tmp)
			break;
		full_path = ft_strjoin(tmp, split_cmd[0]); //free, WHAT IF IT IS NULL
		if (!full_path)
			break;
		free(tmp);
		if (access(full_path, X_OK) == 0) //WHAT IF IT FAIL, WHAT MSG TO PUT?
		{
			if (index == 0)
				p->infile_fd = handle_files(index, p);
			else if (index == 1)
				p->outfile_fd = handle_files(index, p);
			redirect_fds(p, index); //HANDLE ERR
			if (execve(full_path, split_cmd, p->envp) == -1) //HANDLE ERR
			{
				free(full_path);
				break;
			}
		}
		else if (access(full_path, X_OK) == -1)
		{
			if (errno = EACCES)
				p_error(126, "access");
		}
		free(full_path);
		i++;
	}
	return (NULL);
}
char	*handle_child(t_pipex *p, int index)
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
	if (!p->envp[i])
		return (NULL);
	split_paths = ft_split(p->envp[i] + 5, ':');
	if (!split_paths)
			return (NULL);
	if (index == 0)
	{
		split_cmd = ft_split(p->argv[2], ' ');
		if (!split_cmd)
			return (NULL);
	}
	else
	{
		split_cmd = ft_split(p->argv[3], ' ');
		if (!split_cmd)
			return (NULL);
	}
	find_right_path(split_paths, split_cmd, p, index);
	free_split(split_cmd);
	free_split(split_paths);
}
pid_t subprocess(t_pipex *p, int index)
{
	pid_t	id;
	char	*res;
	id = fork();
	if (id == -1) //HANDLE ERR
		p_error(1, "fork");
	if (id == 0)
		res = handle_child(p, index);
	if (!res)
			//maybe call a function to clean and close everthing with right exit code; but what should i clean and close..
	return id;
}
int	main(int argc, char **argv, char **envp)
{
	t_pipex	p;

	if (argc != 5)
		write (2, "Usage: ./pipex infile \"cm1\" \"cm2\" outfile\n", 42);
	p.argv = argv;
	p.envp = envp;
	if (pipe(p.pipefd) == -1) //HANDLE ERR
	{
		p_error(1, "pipe");
	}
	p.pid[0] = subprocess(&p, 0); //first child process
	p.pid[1] = subprocess(&p, 1); //second child process
}
