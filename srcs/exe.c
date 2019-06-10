/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exe.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:10:20 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/06 15:17:59 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int run_command(char *path, char **av, t_shell *shell)
{
	pid_t    father;
	char	*tmp;
	char	*path_tmp;
	
	if (path[0] == '.')
	{
		path_tmp = getcwd(0, 0);
		tmp = ft_super_join(3, path_tmp, "/", path);
		free(path_tmp);
	}
	else
		tmp = ft_strdup(path);
	change_env(shell->shell_env, "_", tmp);
	free(tmp);
	father = fork();
	if (father == 0)
		if (execve(path, av, shell->shell_env) == -1)
			ft_putendl("command failed");
	if (father > 0)
		wait(NULL);
	return (1);
}

int 	alloc_path(char *path, char **tmp)
{
	int n;
	int m;
	int i;
	
	m = -1;
	i = 0;
	while (path[++m] != ':' && path[m])
		;
	if (!(tmp[i] = ft_memalloc(sizeof(char) * (m + 1))))
		return (0);
	ft_strncpy(tmp[i++], path, m);
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
	return (1);
}

char **get_all_path(char **env)
{
	int n;
	int m;
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
	alloc_path(path, tmp);
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
				free(tmp);
				break ;
			}
			free(tmp);
		}
		if (!path[n])
		{
			ft_putstr("minishell: command not found: ");
			ft_putendl(av[0]);
		}
		free_av(path);
	}
	return (1);
}
