#include "minishell.h"

int shell_init(t_shell *shell)
{
	shell->home = ft_strdup(&(environ[5][5]));
	return (1);
}

char *get_one_arg(char *line)
{
	int n;
	char *tmp;

	n = -1;
	while (line[++n] != ' ' && line[n] != '	' && line[n])
		;
	if (!(tmp = ft_memalloc(sizeof(char) * (n + 1))))
		return (0);
	ft_strncpy(tmp, line, n);
	return (tmp);
}

char **get_av(char *line)
{
	int n;
	int m;
	char **tmp;

	n = 0;
	m = 2;
	while (line[++n])
		if ((line[n - 1] == ' ' || line[n - 1] == '	') &&
		(line[n] != ' ' && line[n] != '	' && line[n] != ';'))
		m++;
	if (!(tmp = ft_memalloc(sizeof(char*) * m)))
		return (0);
	tmp[0] = get_one_arg(line);
	n = 0;
	m = 0;
	while (line[++n])
		if ((line[n - 1] == ' ' || line[n - 1] == '	') &&
		(line[n] != ' ' && line[n] != '	' && line[n] != ';'))
			tmp[++m] = get_one_arg(&line[n]);
	return (tmp);
}

int run_command(char *path, char **av)
{
	pid_t	father;

	father = fork();
	if (father == 0)
		execve(path, av, 0);
	if (father > 0)
		wait(NULL);
	return (1);
}

int run_builtins(char *line)
{
	int n;

	n = -1;
	while (ft_strchr(" 	", line[++n]))
		;
	if (!ft_strncmp("env", &line[n], 3))
	{
		ft_print_tables(environ);
		return (0);	
	}
	if (!ft_strncmp("exit", &line[n], 4))
	{
		free(line);
		exit(1);
		return (0);	
	}
	return (1);
}

int run_non_builtin(char *line)
{
	char *command;
	char **av;
	int n;

	n = -1;
	while (ft_strchr(" 	", line[++n]))
		;
	line = &line[n];
	command = get_one_arg(line);
	av = get_av(line);
	if (!access(command, X_OK | F_OK))
		run_command(command, av);
	n = -1;
	while (av[++n])
		free(av[n]);
	free(av);
	free(command);
	return (1);
}

int main(void)
{
	t_shell	*shell;
	char *line;

	if (!(shell = ft_memalloc(sizeof(t_shell))))
		return (-1);
	shell_init(shell);
	while (1)
	{
		write(1, "Super shell: ", 13);
		get_next_line(0, &line);
		if (run_builtins(line))
			run_non_builtin(line);
		free(line);
	}
	return (0);
}
