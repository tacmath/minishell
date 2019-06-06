/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin2.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:07:59 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/06 15:25:44 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

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
		free(shell->last_dir);
		free(shell);
		n = -1;
		while (av[++n])
			free(av[n]);
		free(av);
		exit(1);
		return (0);
	}
	if (!ft_strcmp("cd", av[0]))
	{
		ft_cd(av, shell);
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
