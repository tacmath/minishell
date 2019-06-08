/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   command.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:08:18 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/06 14:10:03 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

char *get_one_arg(char *line)
{
	int n;
	char *tmp;

	n = -1;
	while (line[++n] != ' ' && line[n])
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

	n = -1;
	while (line[++n] && line[n] == ' ')
		;
	line = &line[n];
	n = 0;
	m = 2;
	while (line[++n])
		if (line[n - 1] == ' ' &&
				line[n] != ' ' && line[n] != ';')
			m++;
	if (!(tmp = ft_memalloc(sizeof(char*) * m)))
		return (0);
	tmp[0] = get_one_arg(line);
	n = 0;
	m = 0;
	while (line[++n])
		if (line[n - 1] == ' '  &&
				line[n] != ' '  && line[n] != ';')
			tmp[++m] = get_one_arg(&line[n]);
	return (tmp);
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
