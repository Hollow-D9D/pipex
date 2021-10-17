#include "pipex.h"
#include "libft/libft.h"


void error_argument()
{
	write(2, "Please execute the programm as follows:\n infile cmd cmd.... outfile\n", 68);
	exit(1);
}

void error_files()
{
	write(2, "Error: Infile or Outfile\n", 25);
	exit(1);
}

int check_files(int ac, char** av)
{
	int check;
	
	check = access(av[1], R_OK);
	if (check == -1)
		return 1;
	check = access(av[ac - 1], F_OK);
	if (check == -1)
		return 0;
	check = access(av[ac - 1], W_OK);
	if (check == -1)
		return 1;
	return 0;
}

void parse_args(int ac, char** av, t_general *pipex)
{
	
}

char **get_path(char **env)
{
	int i;

	i = -1;
	while (env[++i])
	{
		//printf("%s\n", env[i]);
		if (!ft_strncmp("PATH=", env[i], 5))
		{
		//	printf("%s\n", env[i]);
			return ft_split(env[i] + 5, ':');
		}
	}
	return (NULL);
}

int main(int ac, char** av, char** env)
{
	t_general pipex;

	if (ac < 4)
		error_argument();
	if (check_files(ac, av))
		error_files();
	pipex.path = get_path(env);
	printf("valod\n");
	for(int i = 0; pipex.path[i]; i++)
	{
		printf("%s\n", pipex.path[i]);
	}
	//parse_args(ac, av, &pipex);
	//exec_commands(pipex);
}
