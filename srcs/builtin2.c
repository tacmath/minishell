/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   builtin2.c                                       .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:07:59 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/11 11:16:27 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

void	ft_echo(char **av)
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

void	free_shell(t_shell *shell)
{
	int n;

	n = -1;
	while (shell->shell_env[++n])
		free(shell->shell_env[n]);
	free(shell->shell_env);
	free(shell->home);
	free(shell->last_dir);
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
