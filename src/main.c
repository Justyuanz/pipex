/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:07:27 by jinzhang          #+#    #+#             */
/*   Updated: 2025/08/12 20:00:22 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	safe_exit(t_pipex *p, char *msg, int exit_code)
{
	close_free_everything(p, msg);
	exit(exit_code);
}

static void	handle_child(t_pipex *p, int index)
{
	int	i;

	i = -1;
	if (index == 0)
		p->infile_fd = handle_files(index, p);
	else if (index == 1)
		p->outfile_fd = handle_files(index, p);
	p->split_cmd = split_command(p, index);
	if (!p->split_cmd)
		safe_exit(p, "Split fail\n", 1);
	if (ft_strchr(p->argv[index + 2], '/') != NULL)
		find_right_path(p, index);
	while (p->envp[++i])
	{
		if (ft_strncmp(p->envp[i], "PATH=", 5) == 0)
			break ;
	}
	if (!p->envp[i])
		safe_exit(p, "command not found\n", 127);
	p->split_paths = ft_split(p->envp[i] + 5, ':');
	if (!p->split_paths)
		safe_exit(p, "Split fail\n", 1);
	find_right_path(p, index);
	free_split(p->split_cmd);
	free_split(p->split_paths);
}

pid_t	subprocess(t_pipex *p, int index)
{
	pid_t	id;

	id = fork();
	if (id == -1)
	{
		perror("fork");
		safe_exit(p, NULL, 1);
	}
	if (id == 0)
		handle_child(p, index);
	return (id);
}

void	init_struct(t_pipex *p, char **argv, char **envp)
{
	p->argv = argv;
	p->envp = envp;
	p->infile_fd = -1;
	p->outfile_fd = -1;
	p->pipefd[0] = -1;
	p->pipefd[1] = -1;
	p->full_path = NULL;
	p->split_cmd = NULL;
	p->split_paths = NULL;
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	p;
	int		status;

	if (argc != 5)
		return (1);
	init_struct(&p, argv, envp);
	if (pipe(p.pipefd) == -1)
	{
		perror("pipe");
		safe_exit(&p, NULL, 1);
	}
	p.pid[0] = subprocess(&p, 0);
	p.pid[1] = subprocess(&p, 1);
	close(p.pipefd[1]);
	close(p.pipefd[0]);
	waitpid(p.pid[0], &status, 0);
	waitpid(p.pid[1], &status, 0);
	if (WIFEXITED(status))
	{
		close_free_everything(&p, NULL);
		return (WEXITSTATUS(status));
	}
	return (1);
}
