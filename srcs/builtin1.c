/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin1.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:07:43 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/05 15:45:41 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

void	cd_last_dir(t_shell *shell, char *tmp)
{
	tmp = shell->last_dir;
	shell->last_dir = getcwd(0, 0);
	chdir(tmp);
	change_env(shell->shell_env, "PWD", tmp);
	change_env(shell->shell_env, "OLDPWD", shell->last_dir);
	free(tmp);
}

int		cd_check(char **av, t_shell *shell, char *tmp)
{
	if (!av[1])
	{
		chdir(shell->home);
		return (1);
	}
	if (av[2])
	{
		ft_putstr("cd: string not in pwd: ");
		ft_putendl(av[1]);
		return (1);
	}
	if (!ft_strcmp(av[1], "-") && shell->last_dir)
	{
		cd_last_dir(shell, tmp);
		return (1);
	}
	if (access(av[1], F_OK))
	{
		ft_putstr("cd: no such file or directory: ");
		ft_putendl(av[1]);
		return (1);
	}
	return (0);
}

int		ft_cd(char **av, t_shell *shell)
{
	char *tmp;

	if (cd_check(av, shell, 0))
		return (1);
	if (access(av[1], R_OK))
	{
		ft_putstr("cd: permission denied: ");
		ft_putendl(av[1]);
		return (1);
	}
	free(shell->last_dir);
	shell->last_dir = getcwd(0, 0);
	if (chdir(av[1]))
	{
		ft_putstr("cd: not a directory: ");
		ft_putendl(av[1]);
	}
	tmp = getcwd(0, 0);
	change_env(shell->shell_env, "PWD", tmp);
	free(tmp);
	change_env(shell->shell_env, "OLDPWD", shell->last_dir);
	return (1);
}

int		env_realloc(char **av, t_shell *shell, int n)
{
	char **tmp;

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

int		ft_setenv(char **av, t_shell *shell)
{
	int len;
	int n;

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
	n = -1;
	while (shell->shell_env[++n])
		if (!ft_strncmp(shell->shell_env[n], av[1], len))
		{
			free(shell->shell_env[n]);
			shell->shell_env[n] = ft_strdup(av[1]);
			return (1);
		}
	env_realloc(av, shell, n);
	return (1);
}
