/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   auto_c.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 15:27:53 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/11/06 16:53:36 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

static int		complete_line(t_shell *shell, int comp_nb)
{
	char	*tmp;
	int		len;

	len = ft_strlen(shell->pre_cursor);
	if (shell->comp->list[comp_nb].type != DT_DIR)
	{
		if (!(tmp = ft_strjoin(shell->pre_cursor,
			shell->comp->list[comp_nb].name)))
			return (0);
	}
	else if (!(tmp = ft_super_join(3, shell->pre_cursor,
				shell->comp->list[comp_nb].name, "/")))
	{
		return (0);
	}
	free(shell->pre_cursor);
	shell->pre_cursor = tmp;
	return (1);
}

void			print_comp(t_shell *shell, t_file file)
{
	int tmp;

	tmp = 0;
	move_cursor(shell, tmp, get_strlen(shell->pre_cursor), 0);
	clear_line(shell, get_strlen(shell->pre_cursor),
		get_strlen(shell->after_cursor));
	write(1, file.name, ft_strlen(file.name));
	if (file.type == DT_DIR)
	{
		write(1, "/", 1);
		tmp = 1;
	}
	if (ft_strlen(shell->after_cursor))
	{
		write(1, shell->after_cursor, ft_strlen(shell->after_cursor));
		move_cursor(shell, get_strlen(shell->pre_cursor)
			+ get_strlen(shell->after_cursor) + ft_strlen(file.name
			- get_strlen("") + tmp), get_strlen(shell->pre_cursor)
			+ ft_strlen(file.name) + tmp, 1);
	}
}

static void		auto_comp_tab(t_shell *shell, int *num)
{
	t_file	*list;
	int		tmp;

	list = shell->comp->list;
	if (*num == -1)
	{
		tmp = get_strlen(shell->pre_cursor);
		shell->pre_cursor[shell->comp->start] = 0;
		ft_putstr("     ");
		ft_putstr(shell->pre_cursor);
	}
	else
		tmp = get_strlen(shell->pre_cursor) + ft_strlen(list[*num].name);
	if (!list[++(*num)].name)
		*num = 0;
	print_comp(shell, list[*num]);
}

static long int	auto_comp_loop(t_shell *shell)
{
	long int	buf;
	int			num;

	num = -1;
	while (1)
	{
		buf = 0;
		if (read(0, &buf, 7) <= 0)
			return (0);
		if (shell->status)
			return (0);
		if (buf == K_TAB)
			auto_comp_tab(shell, &num);
		else
		{
			if (num != -1)
				complete_line(shell, num);
			return (buf);
		}
	}
}

long int		auto_comp(t_shell *shell)
{
	long int	ret;
	t_comp		*comp;

	if (!(comp = ft_memalloc(sizeof(t_comp))))
		return (0);
	shell->comp = comp;
	get_list(shell);
	if (!comp->list[0].name)
		return (free_comp(comp));
	else if (comp->list[0].name && !comp->list[1].name)
	{
		shell->pre_cursor[comp->start] = 0;
		print_comp(shell, comp->list[0]);
		complete_line(shell, 0);
		return (free_comp(comp));
	}
	print_choice(shell);
	ret = auto_comp_loop(shell);
	free_comp(comp);
	return (ret);
}
