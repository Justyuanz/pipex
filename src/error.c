#include "pipex.h"
//a function to print error msg based on error code
//a function to free and exit
//a function to close all fd
//check INFILE & OUTFILE exist and permissions
//check if command exist

void p_error(int exit_code, char *func)
{
	perror(func);
	exit(exit_code);
}