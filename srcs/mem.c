/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   mem.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 14:57:15 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/05 15:06:20 by mtaquet     ###    #+. /#+    ###.fr     */
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

int		next_mem(char **line1, char **line2, int *mem)
{
	t_shell *shell;

	shell = get_shell(0);
	if (shell->mem[*mem + 1])
	{
		(*mem)++;
		free(*line1);
		if (!(*line1 = ft_strdup(shell->mem[*mem])))
			return (0);
		(*line2)[0] = 0;
		tputs(tgoto(tgetstr("ch", 0), 0, get_strlen("")), 1, oputchar);
		tputs(tgetstr("ce", 0), 1, oputchar);
		write(1, *line1, ft_strlen(*line1));
	}
	return (1);
}

int		prev_mem(char **line1, char **line2, int *mem)
{
	t_shell *shell;

	shell = get_shell(0);
	(*line2)[0] = 0;
	if (*mem == 0)
	{
		*mem = -1;
		(*line1)[0] = 0;
		(*line2)[0] = 0;
		tputs(tgoto(tgetstr("ch", 0), 0, get_strlen("")), 1, oputchar);
		tputs(tgetstr("ce", 0), 1, oputchar);
	}
	else if (*mem > 0)
	{
		(*mem)--;
		free(*line1);
		if (!(*line1 = ft_strdup(shell->mem[*mem])))
			return (ft_super_free(1, *line2));
		(*line2)[0] = 0;
		tputs(tgoto(tgetstr("ch", 0), 0, get_strlen("")), 1, oputchar);
		tputs(tgetstr("ce", 0), 1, oputchar);
		write(1, *line1, ft_strlen(*line1));
	}
	return (1);
}
