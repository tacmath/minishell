/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   list.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/09/05 15:00:05 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/09/05 16:55:35 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int			add_to_list(char ***list, char *new)
{
	int		len;
	int		n;
	char	**tmp;

	len = -1;
	while ((*list)[++len])
		if (!ft_strcmp((*list)[len], new))
		{
			free(new);
			return (1);
		}
	if (!(tmp = malloc(sizeof(char*) * (len + 2))))
		return (0);
	n = -1;
	while (++n < len)
		tmp[n] = (*list)[n];
	tmp[n] = new;
	tmp[n + 1] = 0;
	free(*list);
	*list = tmp;
	return (1);
}

void		remove_last_name(char *str)
{
	int n;
	int ret;

	n = -1;
	ret = 0;
	while (str[++n])
		if (str[n] == '/')
			ret = n + 1;
	str[ret] = 0;
}

int			get_list(char *line1, char ***list, int *start)
{
	char *path;
	char *name;
	char type;

	if (!(*list = ft_memalloc(sizeof(char*))))
		return (0);
	path = 0;
	get_line_path(line1, &path, &type, start);
	if (!(name = ft_strdup(&line1[*start])))
		return (0);
	remove_last_name(path);
	if ((!(*start) || line1[*start - 1] == ' '
			|| line1[*start - 1] == '\t') && type)
		get_all_command(list, name);
	else if (type)
		get_all_command_and_dir_from_path(list, path, name);
	else if ((line1[*start - 1] == ' ' || line1[*start - 1] == '\t'))
		get_all_from_path(list, path, name);
	else
		get_all_from_path(list, path, name);
	free(path);
	free(name);
	return (1);
}

int			print_all_choice(char **list, t_shell *shell)
{
	int longest_arg;
	int apl;
	int line;
	int n;

	line = 1;
	n = -1;
	longest_arg = 2;
	while (list[++n])
		if ((int)ft_strlen(list[n]) + 2 > longest_arg)
			longest_arg = ft_strlen(list[n]) + 2;
	if (!(apl = shell->nb_co / longest_arg))
		return (line);
	n = -1;
	while (list[++n])
	{
		tputs(tgoto(tgetstr("ch", 0), 0, (n % apl) * longest_arg), 1, oputchar);
		write(1, list[n], ft_strlen(list[n]));
		if (n % apl == apl - 1)
		{
			tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
			line++;
		}
	}
	return (line);
}
