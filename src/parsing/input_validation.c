#include "pipex.h"
bool file_handling(char **argv)
{
	int	fd1;
	int fd2;

	fd1 = open(argv[1], O_RDONLY);
	if (fd1 == -1)
	{
		perror("infile");
		exit(-1);
	}
	// fd2 = open(argv[4], O_RDWR);
	// if (fd2 == -1)
	// {
	// 	perror("outfile");
	// 	close(fd1);
	// 	exit(-1);
	// }
	return (true);
}