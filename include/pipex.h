/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinzhang <jinzhang@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/11 21:08:48 by jinzhang          #+#    #+#             */
/*   Updated: 2025/08/12 17:42:55 by jinzhang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h> //DELETE ME
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		infile_fd;
	int		outfile_fd;
	int		pipefd[2];
	pid_t	pid[2];

	char	**argv;
	char	**envp;
	char	**split_paths;
	char	**split_cmd;
	char	*full_path;
}			t_pipex;

int			handle_files(int index, t_pipex *p);
void		free_split(char **arr);
void		safe_exit(t_pipex *p, char *msg, int exit_code);
void		close_free_everything(t_pipex *p, char *msg);
void		find_right_path(t_pipex *p, int index);
char		**split_command(t_pipex *p, int index);

#endif
