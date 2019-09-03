
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
	while (line[--n] != '\t' && line[n] == ' ' && n)
		if (line[n] == '/' && (*start) == -1)
			(*start) = n + 1;
	if ((*path)[n] != '~' && (*path)[n + 1] != '/')
	{
		if (!(*path = ft_strdup(&line[n])))
			return (0);
	}
	else if (!(*path = ft_strjoin(get_env("HOME"), &line[n + 1])))              //faire un ft_get_env
		return (0);
	if (*start == -1)
		(*start) = n + 1;
	n = ft_strlen(*path);
	if (is_command(line1))
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

	len = 0;
	while ((*list)[len++])
		;
	if (!(tmp = ft_memalloc(sizeof(char**) * (len + 2))))
		return (0);
	n = -1;
	while (++n < len)
		tmp[n] = (*list)[n];
	if (!(tmp[n] = ft_strdup(new)))
		return (0);
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
			if (!add_to_list(list, info->d_name))
				return (0);
	}
	closedir(dir);
	return (1);
}

int get_all_command(char ***list, char *command)
{
	t_shell *shell = get_shell(0);
	char	builtin[7][9] = {"cd", "env", "echo", "setenv", "unsetenv", "exit", 0};
	char	**path;
	int		n;
	int 	command_len;
	
	command_len = ft_strlen(command);
	n = -1;
	while (builtin[++n])
		if (!ft_strncmp(command, builtin[n] , command_len) && !add_to_list(list, builtin[n]))
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
		if (!ft_strncmp(command, info->d_name, start_len))
			if (!add_to_list(list, info->d_name))
				return (0);
	}
	closedir(dir);
	return (1);
}

void		remove_last_name(char *str)
{
	int n;
	int ret;

	n = -1
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

	if (!(list = ft_memalloc(sizeof(char**))))
		return (0);
	path = 0;
	if (!(name = ft_strdup(&line1[*start])))
		return (0);
	remove_last_name(path);
	get_line_path(line1, &path, &type, start);
	if ((!(*start) || line1[*start] == ' ' || line1[*start] == '\t')&& type)
		get_all_command(list, name);
	else if (type)
		get_all_command_from_path(list, path, name)
	else if ((!(*start) || line1[*start] == ' ' || line1[*start] == '\t'))
		get_all_from_path(list, "./", name)
	else
		get_all_from_path(list, path, name)
	free(path);
	free(name);
	return (1);
}

int 		auto_comp(char **line1, char *line2)
{
	char **list;
	int start;

	start = -1;
	get_list(line1, &list, &start);
	/*tputs(tgoto(tgetstr("ch", 0), 0, 0), 1, oputchar);
	  tputs(tgoto(tgetstr("do", 0), 0, 0), 1, oputchar);
	  tputs(tgetstr("cd", 0), 1, oputchar);
	  */






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
