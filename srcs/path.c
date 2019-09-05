/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   path.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 14:55:24 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/05 16:56:01 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

char		is_command(char *line)
{
	int n;

	n = -1;
	while (line[++n] == ' ' || line[n] == '\t')
		;
	if (!line[n])
		return (2);
	while (line[++n] != ' ' && line[n] != '\t' && line[n])
		;
	if (!line[n])
		return (1);
	return (0);
}

static int	complete_path(char **path, char *line, int n, char type)
{
	if (line[n] != '~' && line[n + 1] != '/')
	{
		if (type == 1 || (line[n] == '/'
			|| (line[n] == '.' && line[n + 1] == '/')))
		{
			if (!(*path = ft_strdup(&line[n])))
				return (0);
		}
		else if (!(*path = ft_strjoin("./", &line[n + 1])))
			return (0);
	}
	else if (!(*path = ft_strjoin(getenv("HOME"), &line[n + 1])))
		return (0);
	return (1);
}

int			get_line_path(char *line, char **path, char *type, int *start)
{
	int n;

	n = ft_strlen(line);
	if (line[n - 1] == ' ' || line[n - 1] == '\t')
	{
		*type = 0;
		(*start) = n;
	}
	while (line[--n] != '\t' && line[n] != ' ' && n)
		if (line[n] == '/' && (*start) == -1)
			(*start) = n + 1;
	if (line[n] == '/' && (*start) == -1)
		(*start) = n + 1;
	if (*start == -1 && n)
		(*start) = n + 1;
	else if (*start == -1)
		(*start) = 0;
	if (is_command(line))
		*type = 1;
	else
		*type = 0;
	complete_path(path, line, n, *type);
	return (1);
}
