/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   main.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:10:54 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/11/05 15:07:05 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

#include <signal.h>

int			treat_line(char *line, t_shell *shell)
{
	char	**av;
	int		n;
	int		m;

	n = -1;
	m = 0;
	while (!(++n) || line[n - 1])
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

static void	ft_sigint(int sig)
{
	t_shell	*shell;

	(void)sig;
	shell = get_shell(0);
	write(1, "\n", 1);
	tputs(tgetstr("cd", 0), 1, oputchar);
	if (!shell->status || shell->status == 2)
	{
		write(1, PROMPT, ft_strlen(PROMPT));
		if (shell->pre_cursor)
			shell->pre_cursor[0] = 0;
		if (shell->after_cursor)
                     	shell->after_cursor[0] = 0;
	}
}

static void	change_win(int sig)
{
	t_shell			*shell;
	struct winsize	size;

	shell = get_shell(0);
	ioctl(0, TIOCGWINSZ, &size);
	if (sig && !ft_strlen(shell->after_cursor))
	{
		if (!(get_strlen((shell->pre_cursor)) % shell->nb_co) && (get_strlen(shell->pre_cursor) % size.ws_col))
		{
			tputs(tgoto(tgetstr("up", 0), 0, 0), 1, oputchar);
			tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(shell->pre_cursor) % size.ws_col), 1, oputchar);
		}
		else if ((get_strlen(shell->pre_cursor) % shell->nb_co) && !(get_strlen(shell->pre_cursor) % size.ws_col))
		{
			tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
			tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(shell->pre_cursor) % size.ws_col), 1, oputchar);
		}
	}
	shell->nb_co = size.ws_col;
	shell->nb_li = size.ws_row;
}

static int	main_loop(t_shell *shell)
{
	char *line;

	while (1)
	{
		write(1, PROMPT, ft_strlen(PROMPT));
		shell->status = 0;
		if (!(line = get_line(shell)))
		{
			free_shell(shell);
			return (0);
		}
		shell->status = 1;
		if (!treat_line(line, shell))
		{
			free_shell(shell);
			return (0);
		}
	}
	return (1);
}

int			main(int ac, char **av, char **env)
{
	t_shell	*shell;

	signal(SIGINT, ft_sigint);
	signal(SIGWINCH, change_win);
	ac = 0;
	if (!(shell = ft_memalloc(sizeof(t_shell))))
		return (-1);
	if (!shell_init(shell, env, av[0]))
	{
		free_shell(shell);
		return (-1);
	}
	get_shell(shell);
	change_win(0);
	if (!main_loop(shell))
		return (-1);
	return (ac);
}
