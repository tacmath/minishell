/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   event.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 12:02:58 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/10/11 14:51:00 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

void		move_cursor(t_shell *shell, int start, int end, char mode)
{
	int line;
	char m;

	m = (mode & 2) >> 1;
	line = (end - m) / shell->nb_co;
	line -= (start - (mode & 1)) / shell->nb_co;
	if (line > 0)
		while (line--)
			tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
	else if (line < 0)
	{
		while (line++)
			tputs(tgoto(tgetstr("up", 0), 0, 0), 1, oputchar);
	}
	tputs(tgoto(tgetstr("ch", 0), 0, ((end - m) % shell->nb_co) + m), 1, oputchar);
}

static char	*return_line(char *line1, char *line2)
{
	char *tmp;

	if (!(tmp = ft_super_join(2, line1, line2)))
	{
		ft_super_free(2, line1, line2);
		return (0);
	}
	write(1, "\n", 1);
	tputs(tgetstr("cd", 0), 1, oputchar);
	ft_super_free(2, line1, line2);
	add_to_mem(tmp);
	return (tmp);
}

static int add_to_line(t_shell *shell, char *new)
{
	char *tmp;
	int tmp_len;

	tmp_len = get_strlen(shell->pre_cursor);
	if (!(tmp = ft_super_join(2,shell->pre_cursor , new)))
		return (ft_super_free(2, shell->pre_cursor, shell->after_cursor));
	free(shell->pre_cursor);
	tputs(tgetstr("cd", 0), 1, oputchar);
	move_cursor(shell, tmp_len, tmp_len, 2);
	write(1, new, ft_strlen(new));
	write(1, shell->after_cursor, ft_strlen(shell->after_cursor));
	tmp_len = get_strlen(tmp);
	move_cursor(shell, tmp_len + get_strlen(shell->after_cursor) - get_strlen(""), tmp_len, 1);
	shell->pre_cursor = tmp;
	return (1);
}

int			use_event(t_shell *shell, long int buf)
{
	if (buf == K_UP)
		next_mem(shell);
	else if (buf == K_DOWN)
		prev_mem(shell);
	else if (buf == K_RIGHT)
		go_to_right(shell);
	else if (buf == K_LEFT)
		go_to_left(shell);
	else if (buf == K_BACKSPACE)
		remove_one_char(shell);
	else if (!add_to_line(shell, (char*)(&buf)))
		return (0);
	return (1);
}

char		*get_line(t_shell *shell)
{
	long int	buf;

	 
	shell->mem_nb = -1;
	if (!(shell->pre_cursor = ft_memalloc(1)) ||
		!(shell->after_cursor = ft_memalloc(1)))
		return (0);
	while (1)
	{
		buf = 0;
		if (read(0, &buf, 7) <= 0)
			return (0);
		if (buf == K_TAB && is_command(shell->pre_cursor) != 2)
			buf = auto_comp(shell);
		if (buf == K_RETURN)
			return (return_line(shell->pre_cursor, shell->after_cursor));
		else if (!use_event(shell, buf))
			return (0);
	}
}
