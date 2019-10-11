/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   list.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 15:00:05 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/11 15:12:27 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int			add_to_list(t_file **list, t_file new)
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

void		remove_last_name(char *str)
{
	int n;
	int ret;

	n = -1;
	ret = 0;
	while (str[++n])
		if (str[n] == '/')
			ret = n + 1;
	str[ret] = 0;
}

int			cmp_list(void *s1, void *s2)
{
	t_file *c1;
	t_file *c2;

	c1 = (t_file*)s1;
	c2 = (t_file*)s2;
	return (ft_strcmp(c1->name, c2->name));
}

void			sort_list(t_file *list)
{
	int n;

	n = -1;
	while (list[++n].name)
		;
	ft_quicksort(list, n, sizeof(t_file), cmp_list);
}

int			get_list(t_shell *shell)
{
	char *path;
	char *name;
	char type;
	int start;

	if (!(shell->comp->list = ft_memalloc(sizeof(t_file))))
		return (0);
	start = -1;
	get_line_path(shell->pre_cursor, &path, &type, &start);
	name = &shell->pre_cursor[start];
	remove_last_name(path);
	if ((!(start) || shell->pre_cursor[start - 1] == ' '
			|| shell->pre_cursor[start - 1] == '\t' ||
			shell->pre_cursor[start - 1] == ';') && type)
		get_all_command(shell, name);
	else if (type)
		get_all_command_and_dir_from_path(&shell->comp->list, path, name);
	else if ((shell->pre_cursor[start - 1] == ' ' || shell->pre_cursor[start - 1] == '\t'))
		get_all_from_path(&shell->comp->list, path, name);
	else
		get_all_from_path(&shell->comp->list, path, name);
	shell->comp->start = start;
	sort_list(shell->comp->list);
	free(path);
	return (1);
}

void			print_file(t_file file)
{
	if (file.type == DT_DIR)
		ft_putstr("\033[1;34m");
	else if (file.type == -1)
		ft_putstr("\033[1;32m");
	else if (file.type != -2)
		ft_putstr("\033[1;31m");
	write(1, file.name, ft_strlen(file.name));
	ft_putstr("\033[m");
	if (file.type == DT_DIR)
		ft_putchar('/');
}

int			print_all_choice(t_shell *shell)
{
	int longest_arg;
	int apl;
	int line;
	int n;
	t_file *list;

	list = shell->comp->list;
	line = 1;
	n = -1;
	longest_arg = 2;
	while (list[++n].name)
		if ((int)ft_strlen(list[n].name) + 2 > longest_arg)
			longest_arg = ft_strlen(list[n].name) + 2;
	if (!(apl = shell->nb_co / longest_arg))
		return (line);
	n = -1;
	while (list[++n].name)
	{
		tputs(tgoto(tgetstr("ch", 0), 0, (n % apl) * longest_arg), 1, oputchar);
		print_file(list[n]);
		if (n % apl == apl - 1)
		{
			tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
			line++;
		}
	}
	return (line);
}
