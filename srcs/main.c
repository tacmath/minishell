/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:10:54 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/11 12:52:15 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

#include <signal.h>

int		g_tes = 1;

void	test(int t)
{
	if (t == 2)
	{
		write(1, "\n", 1);
		if (g_tes == 1)
			write(1, PROMPT, ft_strlen(PROMPT));
	}
}

int oputchar(int c)
{
	return (write(2, &c, 1));
}

int get_strlen(char *str)
{
	int n;
	int len;
	char *prompt;

	prompt = PROMPT;
	len = 0;
	n = -1;
	while (prompt[++n])
	{
		if (prompt[n] == '\t')
			len += 8 - len % 8;
		else
			len++;
	}
	n = -1;
	while (str[++n])
	{
		if (str[n] == '\t')
			len += 8 - len % 8;
		else
			len++;
	}
	return (len);
}

int		free_av(char **av)
{
	int n;

	n = -1;
	while (av[++n])
		free(av[n]);
	free(av);
	return (0);
}

int		treat_line(char *line, t_shell *shell)
{
	char	**av;
	int		n;
	int		m;

	n = 0;
	m = 0;
	while (line[n++])
		if (!line[n] || line[n] == ';')
		{
			if (line[n] == ';')
			{
				line[n] = 0;
				if (!(av = get_av(&line[m])))
					return (0);
				m = ++n;
			}
			else if (!(av = get_av(&line[m])))
				return (0);
			if (treat_av(av, shell) && av[0] && av[0][0])
				if (run_builtins(av, shell))
					run_non_builtin(av, shell);
			free_av(av);
		}
	free(line);
	return (1);
}

char		*return_line(char *line1, char *line2)
{
	char *tmp;	
	
	if (!(tmp = ft_super_join(2, line1, line2)))
	{
		ft_super_free(2, line1, line2);
		return (0);
	}
	write(1, "\n", 1);
	ft_super_free(2, line1, line2);
	return (tmp);
}

char		*add_to_line(char *line1, char *line2, char *new)
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

void	remove_one_char(char *line1, char *line2)
{
	int len;

	len = ft_strlen(line1);
	if (len)
	{
		line1[len - 1] = 0;
		tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(line1)), 1, oputchar);
		tputs(tgetstr("ce", 0), 1, oputchar);
		write(1, line2, ft_strlen(line2));
		tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(line1)), 1, oputchar);
	}
}

int go_to_right(char **line1, char **line2)
{
	int len;
	char *tmp;

	len = ft_strlen(*line2);
	if (len)
	{
		len = ft_strlen(*line1);
		if (!(tmp = ft_memalloc(len + 2)))
		{
			ft_super_free(2, *line1, *line2);
			return (0);
		}
		ft_strcpy(tmp, *line1);
		tmp[len] = (*line2)[0];
		free(*line1);
		*line1 = tmp;
		ft_strcpy(*line2, &(*line2)[1]);
		tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(*line1)), 1, oputchar);
	}
	return (1);
}

int go_to_left(char **line1, char **line2)
{
	int len1;
	int len2;
	char *tmp;

	len1 = ft_strlen(*line1);
	if (len1)
	{
		len2 = ft_strlen(*line2);
		if (!(tmp = ft_memalloc(len2 + 2)))
		{
			ft_super_free(2, *line1, *line2);
			return (0);
		}
		ft_strcpy(&tmp[1], *line2);
		tmp[0] = (*line1)[len1 - 1];
		(*line1)[len1 - 1] = 0;
		free(*line2);
		*line2 = tmp;
		tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(*line1)), 1, oputchar);
	}
	return (1);
}

char		*get_line(void)
{
	char *line1;
	char *line2;
	long int buf;
	
	if (!(line1 = ft_memalloc(1)))
		return (0);
	if (!(line2 = ft_memalloc(1)))
	{
		free(line1);
		return (0);
	}
	while (1)
	{
		buf = 0;
		if (read(0, &buf, 7) <= 0)
			return (0);
		if (buf == K_UP)
			;
		else if	(buf == K_DOWN)
			;
		else if	(buf == K_RIGHT)
			go_to_right(&line1, &line2);
		else if	(buf == K_LEFT)
			go_to_left(&line1, &line2);
		else if	(buf == K_BACKSPACE)
			remove_one_char(line1, line2);
		else if	(buf == K_RETURN)
			return (return_line(line1, line2));
		else if (!(line1 = add_to_line(line1, line2, (char*)(&buf))))
			return (0);
	}
}

int		main(int ac, char **av, char **env)
{
	t_shell	*shell;
	char	*line;

	signal(SIGINT, test);
	ac = 0;
	if (!(shell = ft_memalloc(sizeof(t_shell))))
		return (-1);
	if (!shell_init(shell, env, av[0]))
	{
			free_shell(shell);
			return (-1);
	}
	while (1)
	{
		g_tes = 1;
		write(1, PROMPT, ft_strlen(PROMPT));
		if (!(line = get_line()))
		{
			free_shell(shell);
			return (-1);
		}
		g_tes = 0;
		if (!treat_line(line, shell))
		{
			free_shell(shell);
			return (-1);
		}
	}
	return (ac);
}
