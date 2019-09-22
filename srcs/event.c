/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   event.c                                          .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 12:02:58 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/05 16:42:07 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

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

static char	*add_to_line(char *line1, char *line2, char *new)
{
	char *tmp;

	if (!(tmp = ft_super_join(2, line1, new)))
	{
		ft_super_free(2, line1, line2);
		return (0);
	}
	free(line1);
	tputs(tgetstr("ce", 0), 1, oputchar);
	write(1, new, ft_strlen(new));
	write(1, line2, ft_strlen(line2));
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(tmp)), 1, oputchar);
	return (tmp);
}

int			use_event(t_shell *shell, long int buf)
{
	if (buf == K_UP)
		next_mem(shell);
	else if (buf == K_DOWN)
		prev_mem(shell);
	else if (buf == K_RIGHT)
		go_to_right(&shell->pre_cursor, &shell->after_cursor);
	else if (buf == K_LEFT)
		go_to_left(&shell->pre_cursor, &shell->after_cursor);
	else if (buf == K_BACKSPACE)
		remove_one_char(shell->pre_cursor, shell->after_cursor);
	else if (!(shell->pre_cursor = add_to_line(shell->pre_cursor, shell->after_cursor, (char*)(&buf))))
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
