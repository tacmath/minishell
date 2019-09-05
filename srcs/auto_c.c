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

static int		complete_line(char *comp, char **line1, char *line2, int start)
{
	char *tmp;

	(*line1)[start] = 0;
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(*line1)), 1, oputchar);
	tputs(tgetstr("ce", 0), 1, oputchar);
	if (!(tmp = ft_strjoin(*line1, comp)))
		return (0);
	free(*line1);
	*line1 = tmp;
	write(1, &(*line1)[start], ft_strlen(comp));
	write(1, line2, ft_strlen(line2));
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(*line1)), 1, oputchar);
	return (1);
}

static void		auto_comp_tab(char **list, char *line1, char *line2, int *num)
{
	int n;

	if (*num == -1)
	{
		n = ft_strlen(line1);
		while (n-- && line1[n] != ' ' && line1[n] != '\t')
			if (line1[n] == '\t')
				break ;
		line1[n + 1] = 0;
	}
	if (!list[++(*num)])
		*num = 0;
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(line1)), 1, oputchar);
	tputs(tgetstr("ce", 0), 1, oputchar);
	write(1, list[*num], ft_strlen(list[*num]));
	write(1, line2, ft_strlen(line2));
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(line1)
		+ ft_strlen(list[*num])), 1, oputchar);
}

static long int	auto_comp_loop(char **list,
		char **line1, char *line2, int start)
{
	t_shell		*shell;
	long int	buf;
	int			num;

	shell = get_shell(0);
	num = -1;
	while (1)
	{
		buf = 0;
		if (read(0, &buf, 7) <= 0)
			return (0);
		if (shell->status)
			return (0);
		if (buf == K_TAB)
			auto_comp_tab(list, *line1, line2, &num);
		else
		{
			if (num != -1)
				complete_line(list[num], line1, line2, start);
			return (buf);
		}
	}
}

static void		print_new_prompt(char *line1, char *line2)
{
	tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
	tputs(tgoto(tgetstr("ch", 0), 0, 0), 1, oputchar);
	write(1, PROMPT, ft_strlen(PROMPT));
	write(1, line1, ft_strlen(line1));
	write(1, line2, ft_strlen(line2));
}

long int		auto_comp(char **line1, char *line2, t_shell *shell)
{
	char		**list;
	int			start;
	long int	ret;

	start = -1;
	get_list(*line1, &list, &start);
	if (!list[0])
		return (0);
	else if (list[0] && !list[1])
	{
		complete_line(list[0], line1, line2, start);
		return (0);
	}
	tputs(tgoto(tgetstr("ch", 0), 0, 0), 1, oputchar);
	tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
	tputs(tgetstr("cd", 0), 1, oputchar);
	ret = print_all_choice(list, shell);
	if (ret < shell->nb_li)
		tputs(tgoto(tgetstr("UP", 0), 0, ret), 1, oputchar);
	else
		print_new_prompt(*line1, line2);
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(*line1)), 1, oputchar);
	ret = auto_comp_loop(list, line1, line2, start);
	free_av(list);
	return (ret);
}
