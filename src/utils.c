/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:07:09 by jinzhang          #+#    #+#             */
/*   Updated: 2025/08/11 21:07:13 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	close_free_everything(t_pipex *p, char *msg)
{
	if (msg)
		ft_putstr_fd(msg, 2);
	if (p->infile_fd != -1)
		close(p->infile_fd);
	if (p->outfile_fd != -1)
		close(p->outfile_fd);
	if (p->pipefd[0] != -1)
		close(p->pipefd[0]);
	if (p->pipefd[1] != -1)
		close(p->pipefd[1]);
	if (p->split_paths)
		free_split(p->split_paths);
	if (p->split_cmd)
		free_split(p->split_cmd);
	if (p->full_path)
		free(p->full_path);
}

void	free_split(char **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	while (arr[i])
	{
		free(arr[i]);
		arr[i] = NULL;
		i++;
	}
	free(arr);
	arr = NULL;
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
