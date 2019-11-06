/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   mem.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 14:57:15 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/11/06 15:41:13 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int		add_to_mem(char *line)
{
	t_shell	*shell;
	int		n;

	shell = get_shell(0);
	n = -1;
	while (line[++n] == '\t' || line[n] == ' ')
		;
	if (!line[n] || (shell->mem[0] && !ft_strcmp(line, shell->mem[0])))
		return (1);
	n = -1;
	while (shell->mem[++n])
		;
	if ((n + 1) == MAX_MEM)
		free(shell->mem[n--]);
	while (n-- > 0)
		shell->mem[n + 1] = shell->mem[n];
	if (!(shell->mem[0] = ft_strdup(line)))
		return (1);
	return (1);
}

int		next_mem(t_shell *shell)
{
	if (shell->mem[shell->mem_nb + 1])
	{
		shell->mem_nb++;
		move_cursor(shell, get_strlen(shell->pre_cursor), get_strlen(""), 0);
		free(shell->pre_cursor);
		if (!(shell->pre_cursor = ft_strdup(shell->mem[shell->mem_nb])))
			return (0);
		shell->after_cursor[0] = 0;
		tputs(tgetstr("cd", 0), 1, oputchar);
		write(1, shell->pre_cursor, ft_strlen(shell->pre_cursor));
		move_cursor(shell, get_strlen(shell->pre_cursor),
			get_strlen(shell->pre_cursor), 1);
	}
	return (1);
}

int		prev_mem(t_shell *shell)
{
	shell->after_cursor[0] = 0;
	move_cursor(shell, get_strlen(shell->pre_cursor), get_strlen(""), 0);
	tputs(tgetstr("cd", 0), 1, oputchar);
	if (shell->mem_nb == 0)
	{
		shell->mem_nb = -1;
		shell->pre_cursor[0] = 0;
		shell->after_cursor[0] = 0;
	}
	else if (shell->mem_nb > 0)
	{
		shell->mem_nb--;
		free(shell->pre_cursor);
		if (!(shell->pre_cursor = ft_strdup(shell->mem[shell->mem_nb])))
			return (ft_super_free(1, shell->after_cursor));
		shell->after_cursor[0] = 0;
		write(1, shell->pre_cursor, ft_strlen(shell->pre_cursor));
		move_cursor(shell, get_strlen(shell->pre_cursor),
			get_strlen(shell->pre_cursor), 1);
	}
	return (1);
}
