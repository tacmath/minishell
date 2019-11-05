/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   auto_c.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 15:27:53 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/11/05 16:15:08 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

static int		complete_line(t_shell *shell, int comp_nb)
{
	char *tmp;
	int len;

	len = ft_strlen(shell->pre_cursor);
	if (shell->comp->list[comp_nb].type != DT_DIR)
	{
		if (!(tmp = ft_strjoin(shell->pre_cursor, shell->comp->list[comp_nb].name)))
			return (0);
	}
	else if (!(tmp = ft_super_join(3, shell->pre_cursor, shell->comp->list[comp_nb].name, "/")))
			return (0);
	free(shell->pre_cursor);
	shell->pre_cursor = tmp;
	return (1);
}

void clear_line(t_shell *shell, int start, int end)
{
	int line;
	int n;


	tputs(tgetstr("ce", 0), 1, oputchar);
	if (end < start || !(line = (end - start) / shell->nb_co))
		return ;
	tputs(tgoto(tgetstr("ch", 0), 0, 0), 1, oputchar);
	n = -1;
	while (++n < line)
	{
		tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
		tputs(tgetstr("ce", 0), 1, oputchar);
	}
	tputs(tgoto(tgetstr("UP", 0), 0, line), 1, oputchar);
	tputs(tgoto(tgetstr("ch", 0), 0, start % shell->nb_co), 1, oputchar);
}

static void		auto_comp_tab(t_shell *shell, int *num)
{
	t_file *list;
	char	tmp;

	list = shell->comp->list;
	if (*num == -1)
	{
		tmp = get_strlen(shell->pre_cursor);
		shell->pre_cursor[shell->comp->start] = 0;
	}
	else
		tmp = get_strlen(shell->pre_cursor) + ft_strlen(list[*num].name);
	if (!list[++(*num)].name)
		*num = 0;
	tmp = 0;
	move_cursor(shell, tmp, get_strlen(shell->pre_cursor), 0);
	clear_line(shell, get_strlen(shell->pre_cursor), get_strlen(shell->after_cursor));
	write(1, list[*num].name, ft_strlen(list[*num].name));
	if (list[*num].type == DT_DIR)
	{
		write(1, "/", 1);
		tmp = 1;
	}
	if (ft_strlen(shell->after_cursor))
	{
		write(1, shell->after_cursor, ft_strlen(shell->after_cursor));
		move_cursor(shell, get_strlen(shell->pre_cursor) + get_strlen(shell->after_cursor)
		+ ft_strlen(list[*num].name - get_strlen("") + tmp), get_strlen(shell->pre_cursor) + ft_strlen(list[*num].name) + tmp, 1);
	}
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

static void		print_new_prompt(t_shell *shell)
{
	tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
	tputs(tgoto(tgetstr("ch", 0), 0, 0), 1, oputchar);
	write(1, PROMPT, ft_strlen(PROMPT));
	write(1, shell->pre_cursor, ft_strlen(shell->pre_cursor));
	write(1, shell->after_cursor, ft_strlen(shell->after_cursor));
	move_cursor(shell, get_strlen(shell->pre_cursor) + get_strlen(shell->after_cursor) - get_strlen(""), get_strlen(shell->pre_cursor), 1);
}

int free_comp(t_comp *comp)
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

void print_choice(t_shell *shell)
{
	int line;
	int n;

	line = (get_strlen(shell->after_cursor) - get_strlen("") + get_strlen(shell->pre_cursor) % shell->nb_co) / shell->nb_co;
	tputs(tgoto(tgetstr("ch", 0), 0, 0), 1, oputchar);
	n = line + 1;
	while (n--)
		tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
	tputs(tgetstr("cd", 0), 1, oputchar);
	line += print_all_choice(shell);
	if (line <= shell->nb_li)
		tputs(tgoto(tgetstr("UP", 0), 0, line), 1, oputchar);
	else
		print_new_prompt(shell);
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(shell->pre_cursor) % shell->nb_co), 1, oputchar);
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
		complete_line(shell, 0);
		return (free_comp(comp));
	}
	 print_choice(shell);
	ret = auto_comp_loop(shell);
	free_comp(comp);
	return (ret);
}
