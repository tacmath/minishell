/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils3.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/06 17:03:38 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/11/06 17:16:13 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

static int	alloc_one_path(char *path, char **tmp, int len)
{
	if (!(*tmp = ft_memalloc(sizeof(char) * (len + 2))))
		return (0);
	ft_strncpy(*tmp, path, len);
	if (len && path[len - 1] != '/')
		(*tmp)[len] = '/';
	return (1);
}

static int	alloc_path(char *path, char **tmp)
{
	int n;
	int m;
	int i;

	m = -1;
	i = 0;
	while (path[++m] != ':' && path[m])
		;
	if (!alloc_one_path(path, &tmp[i++], m))
		return (0);
	n = m;
	while (path[n++])
	{
		m = 0;
		while (path[n + ++m] != ':' && path[n + m])
			;
		if (!alloc_one_path(&path[n], &tmp[i++], m))
			return (0);
		n += m;
	}
	return (1);
}

char		**get_all_path(char **env)
{
	int		n;
	int		m;
	char	**tmp;
	char	*path;

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

void		print_file_error(char *error, char *file)
{
	ft_putstr(error);
	ft_putendl(file);
}

int			free_comp(t_comp *comp)
{
	int n;

	if (!comp)
		return (0);
	if (comp->list)
	{
		n = -1;
		while (comp->list[++n].name)
			free(comp->list[n].name);
		free(comp->list);
	}
	free(comp);
	return (0);
}
