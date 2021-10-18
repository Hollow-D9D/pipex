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

void error_fatal()
{
	write(2, "Error: Fatal\n", 13);
	exit(1);
}

int **get_pipes(int count)
{
	int **rtn;
	int i;

	i = -1;
	rtn = (int**)malloc(sizeof(int*) * count);
	while (++i < count)
		rtn[i] = (int*)malloc(sizeof(int) * 2);
	return rtn;
}

void parse_args(int ac, char** av, t_general *pipex)
{
	int	i;
	int	j;

	i = 1;
	j = -1;
	pipex->in_out[0] = dup(0);
	pipex->in_out[1] = dup(1);
	pipex->count = ac - 3;
	pipex->pipes = get_pipes(pipex->count - 1);
	pipex->commands = (t_command *)malloc(sizeof(t_command) * pipex->count);
	if (!pipex->commands)
		error_fatal();
	while (++i < ac - 1)
		pipex->commands[++j].args = ft_split(av[i], ' ');
	pipex->infile = open(av[1], O_RDONLY);
	pipex->outfile = open(av[i], O_CREAT | O_TRUNC | O_WRONLY, 0644);
}

void	ft_redirect(t_general *pipex, int i)
{
	if (!i)
		dup2(pipex->infile, 0);
	else
		dup2(pipex->pipes[i - 1][0], 0);
	if (i + 1 == pipex->count)
		dup2(pipex->outfile, 1);
	else
		dup2(pipex->pipes[i][1], 1);
}

int	ft_execute(t_general *pipex, int n, char **env)
{
	int		rtn;
	int		i;
	char	*tmp;

	i = -1;

	rtn = execve(pipex->commands[n].args[0], pipex->commands[n].args, env);
	while (pipex->path[++i])
	{
		tmp = ft_strjoin(pipex->path[i], "/");
		pipex->commands[n].name = ft_strjoin(tmp, pipex->commands[n].args[0]);
		write(2, pipex->commands[n].name, ft_strlen(pipex->commands[n].name));
		rtn = execve(pipex->commands[n].name, pipex->commands[n].args, env);
		free(pipex->commands[n].name);
		free(tmp);
	}
	return rtn;
}

void	exec_commands(t_general *pipex, char** env)
{
	int pid;
	int i;

	i = -1;
	while(++i < pipex->count)
	{
		if (i + 1 != pipex->count)
			pipe(pipex->pipes[i]);
		pid = fork();
		if (!pid)
		{
			ft_redirect(pipex, i);
			if (ft_execute(pipex, i, env))
			{
				perror("command not found: ");
				exit(1);
			}
		}
	}
	if (pid)
	{
		while(waitpid(-1, NULL, 0))
		{
			if (errno == ECHILD)
				break;
		}
	}
}

char	**get_path(char **env)
{
	int i;

	i = -1;
	while (env[++i])
		if (!ft_strncmp("PATH=", env[i], 5))
			return ft_split(env[i] + 5, ':');
	return (NULL);
}

char	*get_pwd(char **env)
{
	int i;

	i = -1;
	while (env[++i])
		if (!ft_strncmp("PWD=", env[i], 4))
			return (ft_substr(env[i], 4, ft_strlen(env[i]) - 4));
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
	pipex.pwd = get_pwd(env);
	parse_args(ac, av, &pipex);
	exec_commands(&pipex, env);
}
