#include "pipex.h"
int	main(int argc, char **argv, char **env)
{
	int i = 0;
	while (env[i])
		printf("%s", env[i++]);
	if (argc == 5)
	{
		if (file_handling(argv))
		//if files can open, try to 
		{

		}
	write (2, "Usage: ./pipex infile \"cm1\" \"cm2\" outfile\n", 6);
}