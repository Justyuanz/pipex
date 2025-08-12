/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:07:18 by jinzhang          #+#    #+#             */
/*   Updated: 2025/08/12 20:01:24 by jinzhang         ###   ########.fr       */
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
		close(p->infile_fd);
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
		close(p->outfile_fd);
		close(p->pipefd[0]);
		close(p->pipefd[1]);
	}
}

static void	run_command(t_pipex *p, int index)
{
	if (access(p->full_path, X_OK) == 0)
	{
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
		safe_exit(p, "Strjoin fails\n", 1);
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

void	handle_absolute_path(t_pipex *p, int index)
{
	if (ft_strchr(p->argv[index + 2], ' ') != NULL)
		p->argv[index + 2] = p->split_cmd[0];
	p->full_path = ft_strdup(p->argv[index + 2]);
	if (access(p->argv[index + 2], X_OK) == 0)
		run_command(p, index);
	else if (access(p->argv[index + 2], F_OK) == 0)
		safe_exit(p, "Permission denied\n", 126);
	else
		safe_exit(p, "no such file or directory\n", 127);
	free(p->full_path);
	p->full_path = NULL;
}

void	find_right_path(t_pipex *p, int index)
{
	int	i;

	i = -1;
	if (!p->argv[index + 2] || (p->argv[index + 2][0] == ' ' && p->argv[index
			+ 2][1] == '\0'))
		safe_exit(p, "Command not found\n", 127);
	if (ft_strchr(p->argv[index + 2], '/') != NULL)
		handle_absolute_path(p, index);
	else
	{
		while (p->split_paths[++i])
			strjoin_path(p, i, index);
		if (errno == ENOENT)
			safe_exit(p, "Command not found\n", 127);
	}
}
