/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   command.c                                        .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:08:18 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/11 13:20:26 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

char	*get_one_arg(char *line)
{
	int		n;
	char	*tmp;

	n = -1;
	while (line[++n] != ' ' && line[n] != '\t' && line[n])
		;
	if (!(tmp = ft_memalloc(sizeof(char) * (n + 1))))
		return (0);
	ft_strncpy(tmp, line, n);
	return (tmp);
}

char	*check_line(char *line)
{
	int n;

	n = -1;
	while (line[++n] == ' ' || line[n] == '\t')
		;
	return (&line[n]);
}

char	**get_av(char *line)
{
	int		n;
	int		m;
	char	**tmp;

	line = check_line(line);
	if (!line[0])
		return (ft_memalloc(sizeof(char*)));
	n = 0;
	m = 2;
	while (line[++n])
		if ((line[n - 1] == ' ' || line[n - 1] == '\t') &&
			line[n] != ' ' && line[n] != '\t')
			m++;
	if (!(tmp = ft_memalloc(sizeof(char*) * m))
		|| !(tmp[0] = get_one_arg(line)))
		return (0);
	n = 0;
	m = 0;
	while (line[++n])
		if ((line[n - 1] == ' ' || line[n - 1] == '\t') &&
			line[n] != ' ' && line[n] != '\t')
			tmp[++m] = get_one_arg(&line[n]);
	return (tmp);
}

int		treat_dollar(char **av, t_shell *shell, int n)
{
	char	*tmp;
	int		m;

	m = -1;
	while (shell->shell_env[++m])
		if (!ft_strncmp(&av[n][1], shell->shell_env[m], ft_strlen(&av[n][1]))
				&& shell->shell_env[m][ft_strlen(&av[n][1])] == '=')
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
	return (1);
}

int		treat_av(char **av, t_shell *shell)
{
	char	*tmp;
	int		n;

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
			treat_dollar(av, shell, n);
	}
	return (1);
}
