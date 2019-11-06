/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   utils2.c                                         .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/06 16:56:24 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/11/06 17:17:19 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

void		clear_line(t_shell *shell, int start, int end)
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

void		print_new_prompt(t_shell *shell)
{
	tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
	tputs(tgoto(tgetstr("ch", 0), 0, 0), 1, oputchar);
	write(1, PROMPT, ft_strlen(PROMPT));
	write(1, shell->pre_cursor, ft_strlen(shell->pre_cursor));
	write(1, shell->after_cursor, ft_strlen(shell->after_cursor));
	move_cursor(shell, get_strlen(shell->pre_cursor) +
			get_strlen(shell->after_cursor) - get_strlen(""),
			get_strlen(shell->pre_cursor), 1);
}

static void	print_file(t_file file)
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

static int	print_all_choice(t_shell *shell)
{
	int		longest_arg;
	int		apl;
	int		line;
	int		n;
	t_file	*list;

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
		if (n % apl == apl - 1 && list[n + 1].name && line++)
			tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
	}
	return (line);
}

void		print_choice(t_shell *shell)
{
	int line;
	int n;

	line = (get_strlen(shell->after_cursor) - get_strlen("")
			+ get_strlen(shell->pre_cursor) % shell->nb_co) / shell->nb_co;
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
	tputs(tgoto(tgetstr("ch", 0), 0,
				get_strlen(shell->pre_cursor) % shell->nb_co), 1, oputchar);
}
