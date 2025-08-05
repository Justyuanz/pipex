#include "pipex.h"

void safe_exit(t_pipex *p, int exit_code, char *msg)
{
	(void)p;
	if (msg)
		ft_putstr_fd(msg, 2);
	exit(exit_code);
}
void free_split(char **arr)
{
	int	i;

	i = 0;
	while(arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
