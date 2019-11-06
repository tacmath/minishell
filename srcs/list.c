/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   list.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 15:00:05 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/11/06 15:40:42 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int		add_to_list(t_file **list, t_file new)
{
	int		len;
	int		n;
	t_file	*tmp;

	len = -1;
	while ((*list)[++len].name)
		if (!ft_strcmp((*list)[len].name, new.name))
		{
			free(new.name);
			return (1);
		}
	if (!(tmp = malloc(sizeof(t_file) * (len + 2))))
		return (0);
	n = -1;
	while (++n < len)
		tmp[n] = (*list)[n];
	tmp[n] = new;
	tmp[n + 1].name = 0;
	free(*list);
	*list = tmp;
	return (1);
}

int		cmp_list(void *s1, void *s2)
{
	t_file *c1;
	t_file *c2;

	c1 = (t_file*)s1;
	c2 = (t_file*)s2;
	return (ft_strcmp(c1->name, c2->name));
}

void	sort_list(t_file *list)
{
	int n;

	n = -1;
	while (list[++n].name)
		;
	ft_quicksort(list, n, sizeof(t_file), cmp_list);
}

int		get_list(t_shell *shell)
{
	char	*path;
	char	*name;
	char	type;
	int		start;

	if (!(shell->comp->list = ft_memalloc(sizeof(t_file))))
		return (0);
	start = -1;
	get_line_path(shell->pre_cursor, &path, &type, &start);
	name = &shell->pre_cursor[start];
	remove_last_name(path);
	if ((!(start) || shell->pre_cursor[start - 1] == ' ' ||
		shell->pre_cursor[start - 1] == '\t' ||
		shell->pre_cursor[start - 1] == ';') && type)
		get_all_command(shell, name);
	else if (type)
		get_all_command_and_dir_from_path(&shell->comp->list, path, name);
	else if ((shell->pre_cursor[start - 1] == ' '
			|| shell->pre_cursor[start - 1] == '\t'))
		get_all_from_path(&shell->comp->list, path, name);
	else
		get_all_from_path(&shell->comp->list, path, name);
	shell->comp->start = start;
	sort_list(shell->comp->list);
	return (!ft_super_free(1, path));
}
