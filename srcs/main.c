/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:10:54 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/11 12:52:15 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

#include <signal.h>

int		g_tes = 1;

void	test(int t)
{
	if (t == 2)
	{
		write(1, "\n", 1);
		if (g_tes == 1)
			write(1, "Super Shell: ", 13);
	}
}

int		free_av(char **av)
{
	int n;

	n = -1;
	while (av[++n])
		free(av[n]);
	free(av);
	return (0);
}

int		treat_line(char *line, t_shell *shell)
{
	char	**av;
	int		n;
	int		m;

	n = 0;
	m = 0;
	while (line[n++])
		if (!line[n] || line[n] == ';')
		{
			if (line[n] == ';')
			{
				line[n] = 0;
				if (!(av = get_av(&line[m])))
					return (0);
				m = ++n;
			}
			else if (!(av = get_av(&line[m])))
				return (0);
			if (treat_av(av, shell) && av[0] && av[0][0])
				if (run_builtins(av, shell))
					run_non_builtin(av, shell);
			free_av(av);
		}
	free(line);
	return (1);
}

int		main(int ac, char **av, char **env)
{
	t_shell	*shell;
	char	*line;

	signal(SIGINT, test);
	ac = 0;
	if (!(shell = ft_memalloc(sizeof(t_shell))))
		return (-1);
	shell_init(shell, env, av[0]);
	while (1)
	{
		g_tes = 1;
		write(1, "Super Shell: ", 13);
		if (get_next_line(0, &line) < 1)
		{
			free_shell(shell);
			return (-1);
		}
		g_tes = 0;
		if (!treat_line(line, shell))
		{
			free_shell(shell);
			return (-1);
		}
	}
	return (ac);
}
