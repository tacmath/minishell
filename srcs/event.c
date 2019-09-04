
#include "minishell.h"

char        *return_line(char *line1, char *line2)
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

char        *add_to_line(char *line1, char *line2, char *new)
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

char is_command(char *line)
{
	int n;

	n = -1;
	while (line[++n] == ' ' || line[n] == '\t')
		;
	if (!line[n])
		return (2);
	while (line[++n] != ' ' && line[n] != '\t' && line[n])
		;
	if (!line[n])
		return (1);
	return (0);
}

int			get_line_path(char *line, char **path, char *type, int *start)
{
	int n;

	n = ft_strlen(line);

	if (line[n - 1] == ' ')
	{
		*type = 1;
		if (!(*path = ft_strdup("")))
			return (0);
		(*start) = 0;
		return (1);
	}
	while (line[--n] != '\t' && line[n] != ' ' && n)
		if (line[n] == '/' && (*start) == -1)
			(*start) = n + 1;
	if (line[n] != '~' && line[n + 1] != '/')
	{
		if (!(*path = ft_strdup(&line[n])))
			return (0);
	}
	else if (!(*path = ft_strjoin(getenv("HOME"), &line[n + 1])))              //faire un ft_get_env
		return (0);
	if (*start == -1 && n)
		(*start) = n + 1;
	else if (*start == -1)
		(*start) = 0;
	n = ft_strlen(*path);
	if (is_command(line))
		*type = 1;
	else
		*type = 0;
	return (1);
}

int add_to_list(char ***list, char *new)
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

int get_all_command_from_path(char ***list, char *path, char *command)
{
	DIR				*dir;
	struct dirent	*info;
	int				command_len;



	command_len = ft_strlen(command);
	if (!(dir = opendir(path)))
		return (0);
	while ((info = readdir(dir)))
	{
		if (access(info->d_name, X_OK) && !ft_strncmp(command, info->d_name, command_len))
			if (!add_to_list(list, ft_strdup(info->d_name)))
				return (0);
	}
	closedir(dir);
	return (1);
}

char *add_slach_to_name(char *name)
{
	char *tmp;
	int len;

	len = ft_strlen(name);
	if (!(tmp = ft_memalloc(sizeof(char) + len + 2)))
		return (0);
	ft_strcpy(tmp, name);
	tmp[len] = '/';
	return (tmp);
}

int get_all_command_and_dir_from_path(char ***list, char *path, char *command)
{
	DIR				*dir;
	struct dirent	*info;
	int				command_len;

	command_len = ft_strlen(command);
	if (!(dir = opendir(path)))
		return (0);
	while ((info = readdir(dir)))
	{
		if (!ft_strncmp(command, info->d_name, command_len))
		{
			if (info->d_type == DT_DIR)
			{
				if (!add_to_list(list, add_slach_to_name(info->d_name)))
					return (0);
			}
			else if (access(info->d_name, X_OK) && !add_to_list(list, ft_strdup(info->d_name)))
				return (0);
		}
	}
	closedir(dir);
	return (1);
}


int get_all_command(char ***list, char *command)
{
	t_shell *shell = get_shell(0);
	char	builtin[7][9] = {"cd", "env", "echo", "setenv", "unsetenv", "exit"};
	char	**path;
	int		n;
	int 	command_len;

	command_len = ft_strlen(command);
	n = -1;
	while (++n < 6)
		if (!ft_strncmp(command, builtin[n], command_len) && !add_to_list(list, ft_strdup(builtin[n])))
			return (0);
	if (!(path = get_all_path(shell->shell_env)))
		return (0);
	n = -1;
	while (path[++n])
		if (!get_all_command_from_path(list, path[n], command))
			return (0);
	free_av(path);
	return (1);
}

int get_all_from_path(char ***list, char *path, char *start)
{
	DIR				*dir;
	struct dirent	*info;
	int				start_len;


	start_len = ft_strlen(start);
	if (!(dir = opendir(path)))
		return (0);
	while ((info = readdir(dir)))
	{
		if (!ft_strncmp(start, info->d_name, start_len))
		{
			if (info->d_type == DT_DIR)
			{
				if (!add_to_list(list, add_slach_to_name(info->d_name)))
					return (0);
			}
			else if (!add_to_list(list, ft_strdup(info->d_name)))
				return (0);
		}
	}
	closedir(dir);
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
	if ((!(*start) || line1[*start - 1] == ' ' || line1[*start - 1] == '\t') && type)
		get_all_command(list, name);
	else if (type)
		get_all_command_and_dir_from_path(list, path, name);
	else if ((!(*start) || line1[*start - 1] == ' ' || line1[*start - 1] == '\t'))
		get_all_from_path(list, "./", name);
	else
		get_all_from_path(list, path, name);
	free(path);
	free(name);
	return (1);
}

int		print_all_choice(char **list, t_shell *shell)
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
	apl = shell->nb_co / longest_arg;
	if (!apl)
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

int			complete_line(char *comp, char **line1, char *line2, int start)
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

long int	auto_comp_loop(char **list, char **line1, char *line2, int start)
{
	t_shell *shell = get_shell(0);
	long int buf;
	int num;
	
	num = -1;
	while (1)
	{
		buf = 0;
		if (read(0, &buf, 7) <= 0)
			return (0);
		if (shell->status)
		{
			line1[0] = 0;
			line2[0] = 0;
			shell->status = 0;
			return (0);
		}
		if (buf == K_TAB)
		{
			if (num == -1)
				(*line1)[start] = 0;
			if (!list[++num])
				num = 0;
			tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(*line1)), 1, oputchar);
			tputs(tgetstr("ce", 0), 1, oputchar);
			write(1, list[num], ft_strlen(list[num]));
			write(1, line2, ft_strlen(line2));
		}
		else
		{
			if (num != -1)
				complete_line(list[num], line1, line2, start);
			return (buf);
		}
 	}
}

long int auto_comp(char **line1, char *line2, t_shell *shell)
{
	char **list;
	int start;
	long int ret;

	start = -1;
	get_list(*line1, &list, &start);
	if (!list[0])
		return (1);
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
	{
		tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
		tputs(tgoto(tgetstr("ch", 0), 0, 0), 1, oputchar);
		write(1, PROMPT, ft_strlen(PROMPT));
		write(1, *line1, ft_strlen(*line1));
		write(1, line2, ft_strlen(line2));	
	}
	tputs(tgoto(tgetstr("ch", 0), 0, get_strlen(*line1)), 1, oputchar);
	ret = auto_comp_loop(list, line1, line2, start);
	free_av(list);
	return (ret);
}

char        *get_line(void)
{
	t_shell *shell = get_shell(0);
	char    *line1;
	char    *line2;
	int    mem = -1;
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
		if (shell->status)
		{
			line1[0] = 0;
			line2[0] = 0;
			shell->status = 0;
		}
		if (buf == K_TAB && is_command(line1) != 2)
			buf = auto_comp(&line1, line2, shell);
		if (buf == K_UP)
			next_mem(&line1, &line2, &mem);
		else if    (buf == K_DOWN)
			prev_mem(&line1, &line2, &mem);
		else if    (buf == K_RIGHT)
			go_to_right(&line1, &line2);
		else if    (buf == K_LEFT)
			go_to_left(&line1, &line2);
		else if    (buf == K_BACKSPACE)
			remove_one_char(line1, line2);
		else if    (buf == K_RETURN)
			return (return_line(line1, line2));
		else if (!(line1 = add_to_line(line1, line2, (char*)(&buf))))
			return (0);
	}
}
