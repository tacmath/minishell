/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin2.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:07:59 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/11 13:06:32 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int		get_len(char **av, t_shell *shell)
{
	int	len;
	int	n;
	int	m;

	if (!av[1])
		return (-1);
	len = -1;
	while (av[1][++len])
		;
	n = -1;
	m = 0;
	while (shell->shell_env[++n])
		if (!ft_strncmp(shell->shell_env[n], av[1], len) &&
				shell->shell_env[n][len] == '=')
			m = 1;
	if (!m)
	{
		ft_putendl("unsetenv: unexisting variable");
		return (-1);
	}
	return (len);
}

int		ft_unsetenv(char **av, t_shell *shell)
{
	int		len;
	char	**tmp;
	int		n;
	int		m;

	if ((len = get_len(av, shell)) == -1)
		return (1);
	n = -1;
	while (shell->shell_env[++n])
		;
	if (!(tmp = ft_memalloc(sizeof(char*) * n)))
		return (0);
	m = 0;
	n = -1;
	while (shell->shell_env[++n])
		if (!ft_strncmp(shell->shell_env[n], av[1], len)
				&& shell->shell_env[n][len] == '=' && ++m)
			free(shell->shell_env[n]);
		else
			tmp[n - m] = shell->shell_env[n];
	free(shell->shell_env);
	shell->shell_env = tmp;
	return (1);
}

void	ft_echo(char **av)
{
	int n;

	if (av[1])
	{
		ft_putstr(av[1]);
		n = 1;
		while (av[++n])
		{
			ft_putchar(' ');
			ft_putstr(av[n]);
		}
	}
	ft_putchar('\n');
}

void	free_shell(t_shell *shell)
{
	int n;

	n = -1;
	if (shell->shell_env)
	{
		while (shell->shell_env[++n])
			free(shell->shell_env[n]);
		free(shell->shell_env);
	}
	free(shell->home);
	free(shell->last_dir);
	tcsetattr(0, TCSANOW, &shell->old_term);
	free(shell);
}

int		run_builtins(char **av, t_shell *shell)
{
	if (!ft_strcmp("env", av[0]))
		ft_print_tables(shell->shell_env);
	else if (!ft_strcmp("exit", av[0]))
	{
		free_shell(shell);
		free_av(av);
		exit(1);
	}
	else if (!ft_strcmp("cd", av[0]))
		ft_cd(av, shell);
	else if (!ft_strcmp("setenv", av[0]))
		ft_setenv(av, shell);
	else if (!ft_strcmp("unsetenv", av[0]))
		ft_unsetenv(av, shell);
	else if (!ft_strcmp("echo", av[0]))
		ft_echo(av);
	else
		return (1);
	return (0);
}
