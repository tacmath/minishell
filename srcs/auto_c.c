/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   auto_c.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 15:27:53 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/05 16:08:19 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

static int		complete_line(t_shell *shell, int comp_nb)
{
	char *tmp;

	shell->pre_cursor[shell->comp->start] = 0;
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(shell->pre_cursor)), 1, oputchar);
	tputs(tgetstr("ce", 0), 1, oputchar);
	if (!(tmp = ft_strjoin(shell->pre_cursor, shell->comp->list[comp_nb].name)))
		return (0);
	free(shell->pre_cursor);
	shell->pre_cursor = tmp;
	write(1, shell->comp->list[comp_nb].name, ft_strlen(shell->comp->list[comp_nb].name));
	write(1, shell->after_cursor, ft_strlen(shell->after_cursor));
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(shell->pre_cursor)), 1, oputchar);
	return (1);
}

static void		auto_comp_tab(t_shell *shell, int *num)
{
	t_file *list;

	list = shell->comp->list;
	if (*num == -1)
		shell->pre_cursor[shell->comp->start] = 0;
	if (!list[++(*num)].name)
		*num = 0;
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(shell->pre_cursor)), 1, oputchar);
	tputs(tgetstr("ce", 0), 1, oputchar);
	write(1, list[*num].name, ft_strlen(list[*num].name));
	write(1, shell->after_cursor, ft_strlen(shell->after_cursor));
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(shell->pre_cursor)
		+ ft_strlen(list[*num].name)), 1, oputchar);
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
	tputs(tgoto(tgetstr("ch", 0), 0, 0), 1, oputchar);
	tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
	tputs(tgetstr("cd", 0), 1, oputchar);
	ret = print_all_choice(shell);
	if (ret < shell->nb_li)
		tputs(tgoto(tgetstr("UP", 0), 0, ret), 1, oputchar);
	else
		print_new_prompt(shell);
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(shell->pre_cursor)), 1, oputchar);
	ret = auto_comp_loop(shell);
	free_comp(comp);
	return (ret);
}
