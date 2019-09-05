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

int			use_event(char **line1, char **line2, long int buf, int *mem)
{
	if (buf == K_UP)
		next_mem(line1, line2, mem);
	else if (buf == K_DOWN)
		prev_mem(line1, line2, mem);
	else if (buf == K_RIGHT)
		go_to_right(line1, line2);
	else if (buf == K_LEFT)
		go_to_left(line1, line2);
	else if (buf == K_BACKSPACE)
		remove_one_char(*line1, *line2);
	else if (!(*line1 = add_to_line(*line1, *line2, (char*)(&buf))))
		return (0);
	return (1);
}

void		check_status(char *line1, char *line2, t_shell *shell)
{
	if (shell->status)
	{
		line1[0] = 0;
		line2[0] = 0;
		shell->status = 0;
	}
}

char		*get_line(void)
{
	char		*line1;
	char		*line2;
	t_shell		*shell;
	int			mem;
	long int	buf;

	mem = -1;
	shell = get_shell(0);
	if (!(line1 = ft_memalloc(1)) ||
		!(line2 = ft_memalloc(1)))
		return (0);
	while (1)
	{
		buf = 0;
		if (read(0, &buf, 7) <= 0)
			return (0);
		check_status(line1, line2, shell);
		if (buf == K_TAB && is_command(line1) != 2)
			buf = auto_comp(&line1, line2, shell);
		if (buf == K_RETURN)
			return (return_line(line1, line2));
		else if (!use_event(&line1, &line2, buf, &mem))
			return (0);
	}
}
