#include "pipex.h"
bool file_handling(char **argv)
{
	int	fd1;
	int fd2;

	fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1)
	{
		perror("file1");
		return (false);
	}
	fd2 = open(argv[4], O_RDWR);
	if (fd2 == -1)
	{
		perror("file2");
		close(fd1);
		return (false);
	}
	return (true);
}