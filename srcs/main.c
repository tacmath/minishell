#include "minishell.h"

int shell_init(t_shell *shell)
{
	int n;

	shell->home = ft_strdup(&(environ[5][5]));
	n = -1;
	while (environ[++n])
		;
	if (!(shell->shell_env = ft_memalloc(sizeof(char*) * ++n)))
		return (0);
	n = -1;
	while (environ[++n])
		shell->shell_env[n] = ft_strdup(environ[n]);
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

int run_command(char *path, char **av, t_shell *shell)
{
	pid_t	father;

	father = fork();
	if (father == 0)
		if (execve(path, av, shell->shell_env) == -1)
			ft_putendl("command failed");
	if (father > 0)
		wait(NULL);
	return (1);
}

int ft_cd(char **av)
{
	if (!av[1])
		return (1);
	if (av[2])
	{
		ft_putstr("cd: string not in pwd: ");
		ft_putendl(av[1]);
		return (1);
	}
	if (access(av[1], F_OK))
	{
		ft_putstr("cd: no such file or directory: ");
		ft_putendl(av[1]);
		return (1);
	}
	if (access(av[1], R_OK))
	{
		ft_putstr("cd: permission denied: ");
		ft_putendl(av[1]);
		return (1);
	}
	if (chdir(av[1]))
	{
		ft_putstr("cd: not a directory: ");
		ft_putendl(av[1]);
	}
	return (1);
}

int ft_setenv(char **av, t_shell *shell)
{
	int len;
	char **tmp;
	int n;
	int m;

	if (!av[1])
		return (1);
	len = -1;
	while (av[1][++len] != '=' && av[1][len])
		;
	if (!av[1][len])
	{
		ft_putendl("setenv: wrong forma");
		return (1);
	}
	m = 0;
	n = -1;
	while (shell->shell_env[++n])
		if (!ft_strncmp(shell->shell_env[n], av[1], len))
		{
			m = 1;
			free(shell->shell_env[n]);
			shell->shell_env[n] = ft_strdup(av[1]);
		}
	if (m)
		return (1);
	if (!(tmp = ft_memalloc(sizeof(char*) * (n + 2))))
		return (0);
	n = -1;
	while (shell->shell_env[++n])
		tmp[n] = shell->shell_env[n];
	tmp[n] = ft_strdup(av[1]);
	free(shell->shell_env);
	shell->shell_env = tmp;
	return (1);
}

int ft_unsetenv(char **av, t_shell *shell)
{
	int len;
	char **tmp;
	int n;
	int m;

	if (!av[1])
		return (1);
	len = -1;
	while (av[1][++len])
		;
	m = 0;
	n = -1;
	while (shell->shell_env[++n])
		if (!ft_strncmp(shell->shell_env[n], av[1], len) && 
			shell->shell_env[n][len] == '=')
			m = 1;
	if (!m)
	{
		ft_putendl("unsetenv: unexisting variable");
		return (1);
	}
	if (!(tmp = ft_memalloc(sizeof(char*) * n)))
		return (0);
	m = 0;
	n = -1;
	while (shell->shell_env[++n])
	{
		if (!ft_strncmp(shell->shell_env[n], av[1], len) && shell->shell_env[n][len] == '=')
		{
			m++;
			free(shell->shell_env[n]);
		}
		else
			tmp[n - m] = shell->shell_env[n];
	}
	free(shell->shell_env);
	shell->shell_env = tmp;
	return (1);
}

void ft_echo(char **av)
{
	int n;

	ft_putstr(av[1]);
	n = 1;
	while (av[++n])
	{
		ft_putchar(' ');
		ft_putstr(av[n]);	
	}
	ft_putchar('\n');
}

int run_builtins(char **av, t_shell *shell)
{
	int n;

	if (!ft_strcmp("env", av[0]))
	{
		ft_print_tables(shell->shell_env);
		return (0);	
	}
	if (!ft_strcmp("exit", av[0]))
	{
		n = -1;
		while (shell->shell_env[++n])
			free(shell->shell_env[n]);
		free(shell->shell_env);
		free(shell->home);
		n = -1;
		while (av[++n])
			free(av[n]);
		free(av);
		exit(1);
		return (0);
	}
	if (!ft_strcmp("cd", av[0]))
	{
		ft_cd(av);
		return (0);
	}
	if (!ft_strcmp("setenv", av[0]))
	{
		ft_setenv(av, shell);
		return (0);
	}
	if (!ft_strcmp("unsetenv", av[0]))
	{
		ft_unsetenv(av, shell);
		return (0);
	}
	if (!ft_strcmp("echo", av[0]))
	{
		ft_echo(av);
		return (0);
	}
	return (1);
}

char **get_all_path(char **env)
{
	int n;
	int m;
	int i;
	char **tmp;
	char *path;

	n = -1;
	path = 0;
	while (env[++n])
		if (!ft_strncmp("PATH=", env[n], 5))
			path = &env[n][5];
	if (!path)
	{
		if (!(tmp = ft_memalloc(sizeof(char*))))
			return (0);
		return (tmp);
	}
	n = -1;
	m = 2;
	while (path[++n])
		if (path[n] == ':')
			m++;
	if (!(tmp = ft_memalloc(sizeof(char*) * m)))
		return (0);

	m = 0;
	i = 0;
	while (path[n + ++m] != ':' && path[n + m])
		;
	if (!(tmp[i] = ft_memalloc(sizeof(char) * (m + 1))))
		return (0);
	ft_strncpy(tmp[i++], &path[n], m);
	n = -1;
	while (path[++n])
		if (path[n] == ':')
		{
			n++;
			m = 0;
			while (path[n + ++m] != ':' && path[n + m])
				;
			if (!(tmp[i] = ft_memalloc(sizeof(char) * (m + 1))))
				return (0);
			ft_strncpy(tmp[i++], &path[n], m);
		}
	return (tmp);
}

int run_non_builtin(char **av, t_shell *shell)
{
	char **path;
	char *tmp;
	int n;

	if (!access(av[0], X_OK | F_OK))
		run_command(av[0], av, shell);
	else
	{
		path = get_all_path(shell->shell_env);
		n = -1;
		while (path[++n])
		{
			tmp = ft_super_join(3, path[n], "/", av[0]);
			if (!access(tmp, X_OK | F_OK))
			{
				run_command(tmp, av, shell);
				break ;
			}
			free(tmp);
		}
		if (!path[n])
		{
			ft_putstr("minishell: command not found: ");
			ft_putendl(av[0]);
		}
		n = -1;
		while (path[++n])
			free(path[n]);
		free(path);
	}
	return (1);
}

int treat_av(char **av, t_shell *shell)
{
	char *tmp;
	int n;
	int m;

	n = -1;
	while (av[++n])
	{
		if (av[n][0] == '~')
		{
			if (av[n][1] != '/' && av[n][1])
			{
				ft_putstr("minishell: no such user or named directory: ");
				ft_putendl(&av[n][1]);
				return (0);
			}
			tmp = ft_strjoin(shell->home, &av[n][1]);
			free(av[n]);
			av[n] = tmp;
		}
		else if (av[n][0] == '$')
		{
			m = -1;
			while (shell->shell_env[++m])
				if (!ft_strncmp(&av[n][1], shell->shell_env[m], ft_strlen(&av[n][1])))
				{
					tmp = ft_strdup(&shell->shell_env[m][ft_strlen(av[n])]);
					free(av[n]);
					av[n] = tmp;
					break ;
				}
			if (!shell->shell_env[m])
			{
				free(av[n]);
				m = n;
				while (av[++m])
					av[m - 1] = av[m];
				av[m - 1] = av[m];	
			}
		}
	}
	return (1);
}

int main(void)
{
	t_shell	*shell;
	char *line;
	char **av;
	int n;

	if (!(shell = ft_memalloc(sizeof(t_shell))))
		return (-1);
	shell_init(shell);
	while (1)
	{
		write(1, "Super shell: ", 13);
		get_next_line(0, &line);
		av = get_av(line);
		free(line);
		if (treat_av(av, shell))
			if (run_builtins(av, shell))
				run_non_builtin(av, shell);
		n = -1;
		while (av[++n])
			free(av[n]);
		free(av);
	}
	return (0);
}
