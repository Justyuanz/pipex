/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:07:18 by jinzhang          #+#    #+#             */
/*   Updated: 2025/08/11 21:46:01 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	redirect_fds(t_pipex *p, int index)
{
	if (index == 0)
	{
		if (dup2(p->infile_fd, 0) == -1 || dup2(p->pipefd[1], 1) == -1)
		{
			perror("dup2");
			safe_exit(p, NULL, 1);
		}
		close(p->pipefd[0]);
		close(p->pipefd[1]);
	}
	if (index == 1)
	{
		if (dup2(p->pipefd[0], 0) == -1 || dup2(p->outfile_fd, 1) == -1)
		{
			perror("dup2");
			safe_exit(p, NULL, 1);
		}
		close(p->pipefd[0]);
		close(p->pipefd[1]);
	}
}

static int	handle_files(int index, t_pipex *p)
{
	int	fd;

	if (index == 0)
		fd = open(p->argv[1], O_RDONLY);
	else
		fd = open(p->argv[4], O_RDWR | O_CREAT | O_TRUNC);
	if (fd == -1)
	{
		perror("open");
		safe_exit(p, NULL, 1);
	}
	return (fd);
}

static void	run_command(t_pipex *p, int index)
{
	if (access(p->full_path, X_OK) == 0)
	{
		if (index == 0)
			p->infile_fd = handle_files(index, p);
		else if (index == 1)
			p->outfile_fd = handle_files(index, p);
		redirect_fds(p, index);
		if (execve(p->full_path, p->split_cmd, p->envp) == -1)
		{
			free(p->full_path);
			p->full_path = NULL;
			perror("execve");
			safe_exit(p, NULL, 127);
		}
	}
}

static void	strjoin_path(t_pipex *p, int i, int index)
{
	char	*tmp;

	tmp = ft_strjoin(p->split_paths[i], "/");
	if (!tmp)
	{
		safe_exit(p, "Strjoin fails\n", 1);
	}
	p->full_path = ft_strjoin(tmp, p->split_cmd[0]);
	if (!p->full_path)
	{
		free(tmp);
		safe_exit(p, "Strjoin fails\n", 1);
	}
	free(tmp);
	run_command(p, index);
	free(p->full_path);
	p->full_path = NULL;
}

void	find_right_path(t_pipex *p, int index)
{
	int	i;

	i = 0;
	if (access(p->argv[index + 2], X_OK) == 0)
	{
		p->full_path = p->argv[index + 2];
		run_command(p, index);
	}
	while (p->split_paths[i])
	{
		strjoin_path(p, i, index);
		i++;
	}
	if (errno == EACCES)
	{
		perror("access");
		safe_exit(p, "Permission denied\n", 126);
	}
	else
	{
		safe_exit(p, "Command not found\n", 127);
	}
}
